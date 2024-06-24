#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

namespace {
struct InsertSleepPass : public ModulePass {
  static char ID;
  InsertSleepPass() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    LLVMContext &Context = M.getContext();
    IRBuilder<> Builder(Context);
    FunctionCallee SleepFunc = M.getOrInsertFunction("sleep", FunctionType::get(Type::getInt32Ty(Context), {Type::getInt32Ty(Context)}, false));

    bool modified = false;
    for (Function &F : M) {
      if (F.isDeclaration()) continue;

      for (BasicBlock &BB : F) {
        Instruction &FirstInst = *BB.getFirstInsertionPt();
        Builder.SetInsertPoint(&FirstInst);
        Builder.CreateCall(SleepFunc, {Builder.getInt32(1)});
        modified = true;
      }
    }
    return modified;
  }
};

char InsertSleepPass::ID = 0;
static RegisterPass<InsertSleepPass> X("insert-sleep", "Insert sleep(1) into each BasicBlock", false, false);
} // namespace
