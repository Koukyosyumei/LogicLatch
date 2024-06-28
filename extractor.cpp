#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/PostDominators.h>

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <string>

using namespace llvm;

bool isLogicalOperator(Instruction &I)
{
    if (BinaryOperator *BO = dyn_cast<BinaryOperator>(&I))
    {
        switch (BO->getOpcode())
        {
        case Instruction::And:
        case Instruction::Or:
        case Instruction::Xor:
            return true;
        default:
            return false;
        }
    }
    return false;
}

int getDistanceFromEntry(BasicBlock *BB)
{
    // A simple measure of distance from entry (can be improved)
    int distance = 0;
    for (auto *Pred : predecessors(BB))
    {
        distance++;
    }
    return distance;
}

int calculateLifetime(Instruction &I)
{
    // Simplified example of calculating variable lifetime within a block
    // In practice, this would involve data flow analysis across blocks
    if (isa<AllocaInst>(I))
    {
        int start = 0, end = 0;
        for (User *U : I.users())
        {
            if (Instruction *UseInst = dyn_cast<Instruction>(U))
            {
                int position = std::distance(I.getParent()->begin(), BasicBlock::iterator(UseInst));
                if (position > end)
                    end = position;
            }
        }
        return end - start;
    }
    return 0;
}

int calculateCallDepth(CallInst *CI)
{
    // Placeholder for calculating call depth
    // In practice, this requires more complex analysis
    int depth = 0;
    Function *CalledFunc = CI->getCalledFunction();
    if (CalledFunc && !CalledFunc->isDeclaration())
    {
        for (BasicBlock &BB : *CalledFunc)
        {
            for (Instruction &I : BB)
            {
                if (CallInst *NestedCI = dyn_cast<CallInst>(&I))
                {
                    int nestedDepth = calculateCallDepth(NestedCI);
                    if (nestedDepth > depth)
                        depth = nestedDepth;
                }
            }
        }
    }
    return depth + 1; // Account for the current call
}

struct BBFeatures
{
    int numInstructions, distanceFromEntry;
    int numCalls, numLoads, numStores, numArith;
    int numBranches, numCmp, numFP, numLogic, numMemAccess;
    int numPhi, numRet, numPredecessors, numSuccessors;
    int depth, loopNestLevel, numImmediates, numRegisters;
    int numLocalVars, numGlobalVars, numCondBranches, numUncondBranches;
    // int numInt = 0, numDominators = 0, numPostDominators = 0, lifetime = 0, callDepth = 0;

    BBFeatures() : numInstructions(0), distanceFromEntry(0), numCalls(0), numLoads(0), numStores(0),
                   numArith(0), numBranches(0), numCmp(0), numFP(0), numLogic(0), numMemAccess(0), numPhi(0),
                   numRet(0), numPredecessors(0), numSuccessors(0), depth(0), loopNestLevel(0), numImmediates(0),
                   numRegisters(0), numLocalVars(0), numGlobalVars(0), numCondBranches(0), numUncondBranches(0) {}
};

struct FeatureExtractionPass
{
    const std::string CFG_FILE_NAME, FEATURES_FILE_NAME;
    std::ofstream CFG_FILE, FEATURES_FILE;

    // Map to store Basic Block index
    std::unordered_map<BasicBlock *, int> BlockIndex;
    // Assign index to each Basic Block
    int index = 0;
    std::unordered_map<Function *, BasicBlock *> FunctonEntryBlock;

    FeatureExtractionPass(const std::string CFG_FILE_NAME, const std::string FEATURES_FILE_NAME) : CFG_FILE_NAME(CFG_FILE_NAME),
                                                                                                   FEATURES_FILE_NAME(FEATURES_FILE_NAME)
    {
    }

    void open()
    {
        CFG_FILE.open(CFG_FILE_NAME, std::ios::out);
        FEATURES_FILE.open(FEATURES_FILE_NAME, std::ios::out);
        FEATURES_FILE << "FunctionName" << "," << "BasicBlockIndex" << "," << "BasicBlockName" << "," << "numInstructions" << ","
                      << "distanceFromEntry" << "," << "numCalls" << "," << "numLoads" << ","
                      << "numStores" << "," << "numArith" << "," << "numBranches" << ","
                      << "numCmp" << "," << "numFP" << "," << "numLogic" << ","
                      << "numMemAccess" << "," << "numPhi" << "," << "numRet" << "," << "numPredecessors" << ","
                      << "numSuccessors" << "," << "depth" << "," << "loopNestLevel" << ","
                      << "numImmediates" << "," << "numRegisters" << "," << "numLocalVars" << ","
                      << "numGlobalVars" << "," << "numCondBranches" << "," << "numUncondBranches" << "\n";
    }

    void close()
    {
        CFG_FILE.close();
        FEATURES_FILE.close();
    }

    void extractCFG(Function &F)
    {
        // Write control flow edges to file
        for (BasicBlock &BB : F)
        {
            int srcIndex = BlockIndex[&BB];
            for (BasicBlock *Succ : successors(&BB))
            {
                int destIndex = BlockIndex[Succ];
                CFG_FILE << srcIndex << " " << destIndex << "\n";
            }

            for (Instruction &I : BB)
            {
                if (auto *callInst = dyn_cast<CallInst>(&I))
                {
                    Function *callee = callInst->getCalledFunction();
                    if (callee)
                    {
                        CFG_FILE << srcIndex << " " << BlockIndex[FunctonEntryBlock[callee]] << "\n";
                    }
                }
            }
        }
    }

    void extractFeatures(Function &F)
    {
        // Loop and Post Dominator analysis
        // LoopInfo LI;
        // LoopInfoWrapperPass LIWP;
        // LIWP.releaseMemory();
        // LIWP.runOnFunction(F);
        // PostDominatorTree PDT;
        // PostDominatorTreeWrapperPass PDTWP;
        // PDTWP.releaseMemory();
        // PDTWP.runOnFunction(F);

        // ------------- Extract CFG --------------
        for (BasicBlock &BB : F)
        {
            FunctonEntryBlock[&F] = &BB;
            break;
        }

        for (BasicBlock &BB : F)
        {
            BlockIndex[&BB] = index++;
        }

        // ----------------------------------------

        // ------------- Extract features -----------

        for (BasicBlock &BB : F)
        {
            BBFeatures bbf;

            // Feature 1: Number of instructions in the basic block
            bbf.numInstructions = BB.size();
            // Feature 2: Distance from entry point (simple measure)
            bbf.distanceFromEntry = getDistanceFromEntry(&BB);

            for (Instruction &I : BB)
            {
                if (isa<CallInst>(I))
                    bbf.numCalls++;
                if (isa<LoadInst>(I))
                    bbf.numLoads++;
                if (isa<StoreInst>(I))
                    bbf.numStores++;
                if (isa<BinaryOperator>(I))
                    bbf.numArith++;
                if (isa<BinaryOperator>(I) && I.getType()->isPointerTy())
                    bbf.numMemAccess++;
                if (isa<BinaryOperator>(I) && isLogicalOperator(I))
                    bbf.numLogic++;
                if (isa<ICmpInst>(I))
                    bbf.numCmp++;
                if (isa<FCmpInst>(I))
                    bbf.numFP++;
                if (isa<BranchInst>(I))
                {
                    bbf.numBranches++;
                    if (cast<BranchInst>(I).isConditional())
                        bbf.numCondBranches++;
                    else
                        bbf.numUncondBranches++;
                }
                if (isa<PHINode>(I))
                    bbf.numPhi++;
                if (isa<ReturnInst>(I))
                    bbf.numRet++;
                if (isa<AllocaInst>(I))
                    bbf.numLocalVars++;
                if (isa<GlobalVariable>(I))
                    bbf.numGlobalVars++;

                for (auto &Op : I.operands())
                {
                    if (isa<ConstantInt>(Op))
                        bbf.numImmediates++;
                    if (isa<Instruction>(Op))
                        bbf.numRegisters++;
                }

                /*
                // Lifetime calculation
                if (isa<AllocaInst>(I))
                {
                    int varLifetime = calculateLifetime(I);
                    if (varLifetime > lifetime)
                    {
                        lifetime = varLifetime;
                    }
                }

                // Call depth calculation
                if (isa<CallInst>(I))
                {
                    int depth = calculateCallDepth(dyn_cast<CallInst>(&I));
                    if (depth > callDepth)
                    {
                        callDepth = depth;
                    }
                }
                */
            }

            bbf.numPredecessors = std::distance(pred_begin(&BB), pred_end(&BB));
            bbf.numSuccessors = std::distance(succ_begin(&BB), succ_end(&BB));

            /*
            // Depth calculation
            bbf.depth = PDT.getNode(&BB)->getLevel();

            // Loop nest level calculation
            int maxLoopDepth = 0;
            for (auto *L : LI.getLoopsInPreorder()) {
                if (L->contains(&BB)) {
                    maxLoopDepth = std::max(maxLoopDepth, (int)LI.getLoopDepth(&BB));
                }
            }
            bbf.loopNestLevel = maxLoopDepth;
            */

            int numInstructions, distanceFromEntry;
            int numCalls, numLoads, numStores, numArith;
            int numBranches, numCmp, numFP, numLogic, numMemAccess;
            int numPhi, numRet, numPredecessors, numSuccessors;
            int depth, loopNestLevel, numImmediates, numRegisters;
            int numLocalVars, numGlobalVars, numCondBranches, numUncondBranches;

            FEATURES_FILE << F.getName().str() << "," << BlockIndex[&BB] << "," << BB.getName().str() << "," << bbf.numInstructions << ","
                          << bbf.distanceFromEntry << "," << bbf.numCalls << "," << bbf.numLoads << ","
                          << bbf.numStores << "," << bbf.numArith << "," << bbf.numBranches << ","
                          << bbf.numCmp << "," << bbf.numFP << "," << bbf.numLogic << ","
                          << bbf.numMemAccess << "," << bbf.numPhi << "," << bbf.numRet << "," << bbf.numPredecessors << ","
                          << bbf.numSuccessors << "," << bbf.depth << "," << bbf.loopNestLevel << ","
                          << bbf.numImmediates << "," << bbf.numRegisters << "," << bbf.numLocalVars << ","
                          << bbf.numGlobalVars << "," << bbf.numCondBranches << "," << bbf.numUncondBranches << "\n";

            // Other features can be similarly computed...
        }
    }
};

int main(int argc, char **argv)
{
    LLVMContext context;
    SMDiagnostic SMD;

    // Load the module from file
    std::unique_ptr<Module> module = parseIRFile(argv[1], SMD, context);

    if (!module)
    {
        SMD.print(argv[0], errs());
        return 1;
    }

    std::string BASE_FILE_NAME = argv[1];
    size_t last_dot = BASE_FILE_NAME.find_last_of('.');
    if (last_dot != std::string::npos) {
        BASE_FILE_NAME = BASE_FILE_NAME.substr(0, last_dot);
    }
    std::string OUTPUT_DIR = argv[2];

    const std::string CFG_FILE_NAME = OUTPUT_DIR + BASE_FILE_NAME + "-cfg.txt";
    const std::string FEATURES_FILE_NAME = OUTPUT_DIR + BASE_FILE_NAME + "-features.txt";

    FeatureExtractionPass fep(CFG_FILE_NAME, FEATURES_FILE_NAME);
    fep.open();

    for (auto &F : *module)
    {
        fep.extractFeatures(F);
    }

    for (auto &F : *module)
    {
        fep.extractCFG(F);
    }

    fep.close();

    return 0;
}
