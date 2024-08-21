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
            int num_cond_u8_sequence = 0;
            int num_strcmp_calls = 0;
            int num_strncmp_calls = 0;
            int num_memcmp_calls = 0;

            // Loop through each basic block in the function
            for (BasicBlock &BB : F)
            {
                // Loop through each instruction in the basic block
                for (Instruction &I : BB)
                {
                    // Check if the instruction is a CallInst
                    if (CallInst *CI = dyn_cast<CallInst>(&I))
                    {
                        Function *calledFunc = CI->getCalledFunction();
                        if (calledFunc)
                        {
                            StringRef funcName = calledFunc->getName();

                            // Check for strcmp
                            if (funcName == "strcmp")
                            {
                                num_strcmp_calls++;
                            }
                            else if (funcName == "strncmp")
                            {
                                num_strncmp_calls++;
                            }
                            else if (funcName == "memcmp")
                            {
                                num_memcmp_calls++;
                            }
                        }
                    }

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
                            else if (opA->getType()->isPointerTy() && opB->getType()->isPointerTy())
                            {
                                if (opA->getType()->getPointerElementType()->isIntegerTy(8) &&
                                    opB->getType()->getPointerElementType()->isIntegerTy(8))
                                {
                                    num_cond_u8_sequence++;
                                }
                                else
                                {
                                    num_cond_others++;
                                }
                            }
                            else
                            {
                                num_cond_others++;
                            }
                        }
                    }
                }
            }

            errs() << F.getName() << "," << num_cond_integer32 << "," << num_cond_float << ","
                   << num_cond_double << "," << num_cond_u8_sequence << "," << num_strcmp_calls << "," 
                   << num_strncmp_calls << "," << num_memcmp_calls << "\n";

            // Since this pass does not modify the function, return false
            return false;
        }
    };
} // namespace

char BranchExtractorPass::ID = 0;
static RegisterPass<BranchExtractorPass> X("branchextractor", "Branch Instruction Extractor Pass", false, false);
