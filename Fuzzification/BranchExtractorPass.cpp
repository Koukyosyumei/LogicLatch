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
            int num_cond_double = 0;
            int num_cond_integer32 = 0;
            int num_cond_float = 0;
            int num_cond_others = 0;

            // Loop through each basic block in the function
            for (BasicBlock &BB : F)
            {
                // Loop through each instruction in the basic block
                for (Instruction &I : BB)
                {
                    // Check if the instruction is a BranchInst
                    if (BranchInst *BI = dyn_cast<BranchInst>(&I))
                    {

                        if (BI->isConditional() && isa<CmpInst>(BI->getCondition()))
                        {
                            CmpInst *CI = dyn_cast<CmpInst>(BI->getCondition());
                            auto *opA = CI->getOperand(0);
                            auto *opB = CI->getOperand(1);

                            // assuming both operands are of the same type for conditional
                            if (opA->getType()->isDoubleTy())
                            {
                                num_cond_double++;
                            }
                            else if (opA->getType()->isIntegerTy(32))
                            {
                                num_cond_integer32++;
                            }
                            else if (opA->getType()->isFloatTy())
                            {
                                num_cond_float++;
                            }
                            else
                            {
                                num_cond_others++;
                            }
                        }
                    }
                }
            }

            errs() << F.getName() << "," << num_cond_integer32 << "," << num_cond_float << "," << num_cond_double << "," << num_cond_others << "\n";

            // Since this pass does not modify the function, return false
            return false;
        }
    };
} // namespace

char BranchExtractorPass::ID = 0;
static RegisterPass<BranchExtractorPass> X("branchextractor", "Branch Instruction Extractor Pass", false, false);
