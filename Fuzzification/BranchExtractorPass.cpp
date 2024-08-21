#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace
{
    struct BranchExtractorPass : public FunctionPass
    {
        static char ID;
        BranchExtractorPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override
        {
            errs() << "Function: " << F.getName() << "\n";

            // Loop through each basic block in the function
            for (BasicBlock &BB : F)
            {
                // Loop through each instruction in the basic block
                for (Instruction &I : BB)
                {
                    // Check if the instruction is a BranchInst
                    if (BranchInst *BI = dyn_cast<BranchInst>(&I))
                    {
                        errs() << "  Found BranchInst: " << *BI << "\n";

                        if (BI->isConditional() && isa<CmpInst>(BI->getCondition()))
                        {
                            CmpInst *CI = dyn_cast<CmpInst>(BI->getCondition());
                            auto *opA = CI->getOperand(0);
                            auto *opB = CI->getOperand(1);

                            // assuming both operands are of the same type for conditional
                            errs() << "    Conditional Branch: \n";
                            if (opA->getType()->isDoubleTy())
                            {
                                errs() << "      double\n";
                            }
                            else if (opA->getType()->isIntegerTy(32))
                            {
                                errs() << "      integer\n";
                            }
                            else if (opA->getType()->isFloatTy())
                            {
                                errs() << "      float\n";
                            }
                            else
                            {
                                errs() << "      other types\n";
                            }
                        }
                    }
                }
            }

            // Since this pass does not modify the function, return false
            return false;
        }
    };
} // namespace

char BranchExtractorPass::ID = 0;
static RegisterPass<BranchExtractorPass> X("branchextractor", "Branch Instruction Extractor Pass", false, false);
