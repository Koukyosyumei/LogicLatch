#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct BranchExtractorPass : public FunctionPass {
  static char ID;
  BranchExtractorPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << "Function: " << F.getName() << "\n";

    // Loop through each basic block in the function
    for (BasicBlock &BB : F) {
      // Loop through each instruction in the basic block
      for (Instruction &I : BB) {
        // Check if the instruction is a BranchInst
        if (BranchInst *BI = dyn_cast<BranchInst>(&I)) {
          errs() << "  Found BranchInst: " << *BI << "\n";
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
