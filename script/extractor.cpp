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

const std::string CFG_FILE_NAME = "cfg.txt";
const std::string FEATURES_FILE_NAME = "features.txt";
static cl::opt<std::string> InputFilename(cl::Positional, cl::desc("<input LLVM IR file>"), cl::init("-"));

struct FeatureExtractionPass : public FunctionPass
{
    static char ID;
    FeatureExtractionPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override
    {
        // Loop and Post Dominator analysis
        LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
        PostDominatorTree &PDT = getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();

        // ------------- Extract CFG --------------

        // Map to store Basic Block index
        std::unordered_map<BasicBlock *, int> BlockIndex;
        // Assign index to each Basic Block
        int index = 0;
        for (BasicBlock &BB : F)
        {
            BlockIndex[&BB] = index++;
        }
        // Write control flow edges to file
        std::ofstream CFG_FILE;
        CFG_FILE.open(CFG_FILE_NAME, std::ios::out);
        for (BasicBlock &BB : F)
        {
            int srcIndex = BlockIndex[&BB];
            for (BasicBlock *Succ : successors(&BB))
            {
                int destIndex = BlockIndex[Succ];
                CFG_FILE << srcIndex << " " << destIndex << "\n";
            }
        }
        CFG_FILE << "\n";
        CFG_FILE.close();

        // ----------------------------------------

        // ------------- Extract features -----------

        std::ofstream FEATURES_FILE;
        FEATURES_FILE.open(FEATURES_FILE_NAME, std::ios::out);
        FEATURES_FILE << "BasicBlockIndex" << ", " << "BasicBlockName" << ", " << "numInstructions" << ", "
                      << "distanceFromEntry" << ", " << "numCalls" << ", " << "numLoads" << ", "
                      << "numStores" << ", " << "numArith" << ", " << "numBranches" << ", "
                      << "numCmp" << ", " << "numFP" << ", " << "numLogic" << ", "
                      << "numMemAccess" << ", " << "numPhi" << ", " << "numRet" << ", " << "numPredecessors" << ", "
                      << "numSuccessors" << ", " << "depth" << ", " << "loopNestLevel" << ", "
                      << "numImmediates" << ", " << "numRegisters" << ", " << "numLocalVars" << ", "
                      << "numGlobalVars" << ", " << "numCondBranches" << ", " << "numUncondBranches" << "\n";

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
            bbf.depth = PDT.getNode(&BB)->getLevel();
            bbf.loopNestLevel = LI.getLoopDepth(&BB);

            int numInstructions, distanceFromEntry;
            int numCalls, numLoads, numStores, numArith;
            int numBranches, numCmp, numFP, numLogic, numMemAccess;
            int numPhi, numRet, numPredecessors, numSuccessors;
            int depth, loopNestLevel, numImmediates, numRegisters;
            int numLocalVars, numGlobalVars, numCondBranches, numUncondBranches;

            FEATURES_FILE << BlockIndex[&BB] << ", " << BB.getName().str() << ", " << bbf.numInstructions << ", "
                          << bbf.distanceFromEntry << ", " << bbf.numCalls << ", " << bbf.numLoads << ", "
                          << bbf.numStores << ", " << bbf.numArith << ", " << bbf.numBranches << ", "
                          << bbf.numCmp << ", " << bbf.numFP << ", " << bbf.numLogic << ", "
                          << bbf.numMemAccess << ", " << bbf.numPhi << ", " << bbf.numRet << ", " << bbf.numPredecessors << ", "
                          << bbf.numSuccessors << ", " << bbf.depth << ", " << bbf.loopNestLevel << ", "
                          << bbf.numImmediates << ", " << bbf.numRegisters << ", " << bbf.numLocalVars << ", "
                          << bbf.numGlobalVars << ", " << bbf.numCondBranches << ", " << bbf.numUncondBranches << "\n";

            // Other features can be similarly computed...
        }

        FEATURES_FILE.close();

        return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override
    {
        AU.addRequired<LoopInfoWrapperPass>();
        AU.addRequired<PostDominatorTreeWrapperPass>();
        AU.setPreservesAll();
    }
};

char FeatureExtractionPass::ID = 0;
static RegisterPass<FeatureExtractionPass> X("featureextract", "Feature Extraction Pass",
                                             false /* Only looks at CFG */,
                                             false /* Analysis Pass */);

int main(int argc, char **argv)
{
    LLVMContext context;
    SMDiagnostic SMD;

    // Parse command line options
    cl::ParseCommandLineOptions(argc, argv, "Feature Extraction Tool");

    // Load the module from file
    std::unique_ptr<Module> module = parseIRFile(InputFilename, SMD, context);
    if (!module)
    {
        SMD.print(argv[0], errs());
        return 1;
    }

    // Initialize PassManager and add the feature extraction pass
    legacy::PassManager PM;
    PM.add(new FeatureExtractionPass());

    // Run the pass on the module
    PM.run(*module.get());

    return 0;
}
