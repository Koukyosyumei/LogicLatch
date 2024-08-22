#include "llvm/IR/Function.h"
#include <llvm/IR/Module.h>
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "crc.cpp"

using namespace llvm;

namespace
{
  struct CRCPass : public FunctionPass
  {
    static char ID;
    CRCPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override
    {

      // Define used function
      LLVMContext &Ctx = F.getContext();

      // type
      std::vector<Type *> IntParamType = {Type::getInt32Ty(Ctx)};
      Type *IntRetType = Type::getInt32Ty(Ctx);
      std::vector<Type *> DoubleParamType = {Type::getDoubleTy(Ctx)};
      Type *DoubleRetType = Type::getDoubleTy(Ctx);
      std::vector<Type *> FloatParamType = {Type::getFloatTy(Ctx)};
      Type *FloatRetType = Type::getFloatTy(Ctx);
      std::vector<Type *> BoolParamType = {Type::getInt8Ty(Ctx)};
      Type *BoolRetType = Type::getInt8Ty(Ctx);

      // Functions for modifying operand in branch
      FunctionType *ModIntType = FunctionType::get(IntRetType, IntParamType, false);
      Constant *ModIntFunc = llvm::cast<llvm::Constant>(F.getParent()->getOrInsertFunction("modifyInt", ModIntType).getCallee());
      FunctionType *ModDoubleType = FunctionType::get(DoubleRetType, DoubleParamType, false);
      Constant *ModDoubleFunc = llvm::cast<llvm::Constant>(F.getParent()->getOrInsertFunction("modifyDouble", ModDoubleType).getCallee());
      FunctionType *ModFloatType = FunctionType::get(FloatRetType, FloatParamType, false);
      Constant *ModFloatFunc = llvm::cast<llvm::Constant>(F.getParent()->getOrInsertFunction("modifyFloat", ModFloatType).getCallee());
      FunctionType *ModBoolType = FunctionType::get(BoolRetType, BoolParamType, false);
      Constant *ModBoolFunc = llvm::cast<llvm::Constant>(F.getParent()->getOrInsertFunction("modifyBool", ModBoolType).getCallee());

      // Functions for wrapping return operand
      FunctionType *WrapIntType = FunctionType::get(IntRetType, IntParamType, false);
      Constant *WrapIntFunc = llvm::cast<llvm::Constant>(F.getParent()->getOrInsertFunction("retWrapInt", WrapIntType).getCallee());
      FunctionType *WrapDoubleType = FunctionType::get(DoubleRetType, DoubleParamType, false);
      Constant *WrapDoubleFunc = llvm::cast<llvm::Constant>(F.getParent()->getOrInsertFunction("retWrapDouble", WrapDoubleType).getCallee());
      FunctionType *WrapFloatType = FunctionType::get(FloatRetType, FloatParamType, false);
      Constant *WrapFloatFunc = llvm::cast<llvm::Constant>(F.getParent()->getOrInsertFunction("retWrapFloat", WrapFloatType).getCallee());

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
              Constant *currentConstantFunc = NULL;

              auto *opA = CI->getOperand(0);
              auto *opB = CI->getOperand(1);
              auto argsA = llvm::ArrayRef<llvm::Value *>(&opA, 1);
              auto argsB = llvm::ArrayRef<llvm::Value *>(&opB, 1);

              bool modify = false;

              // assuming both operands are of the same type for conditional
              if (opA->getType()->isDoubleTy())
              {
                currentConstantFunc = ModDoubleFunc;
                modify = true;
              }
              else if (opA->getType()->isIntegerTy(32))
              {
                currentConstantFunc = ModIntFunc;
                modify = true;
              }
              else if (opA->getType()->isFloatTy())
              {
                currentConstantFunc = ModFloatFunc;
                modify = true;
              }

              auto *Func = llvm::dyn_cast<llvm::Function>(currentConstantFunc);
              if (modify)
              {
                auto *ciA = llvm::CallInst::Create(Func, argsA, "funcA", CI);
                auto *ciB = llvm::CallInst::Create(Func, argsB, "funcB", CI);

                StringRef handler = cast<CallInst>(ciA)->getCalledFunction()->getName();
                CI->setOperand(0, ciA);
                CI->setOperand(1, ciB);
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

char CRCPass::ID = 0;
static RegisterPass<CRCPass> X("crc-pass", "Branch Obfuscation with CRC", false, false);
