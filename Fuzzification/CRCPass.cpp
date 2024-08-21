#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constant.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstIterator.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/ADT/Statistic.h"
#include "crc.h"

#include <unordered_map>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace llvm;
using namespace std;

cl::opt<std::string> FName("crc-log-fname", cl::desc("Path to the log file"), cl::init("./log.txt"));

namespace {
  struct CRCPass: public FunctionPass {
    static char ID;
    std::ofstream OUTPUT_FILE;

    CRCPass() : FunctionPass(ID) {
      parse_inst_bb();
    }

    void parse_inst_bb(){
      OUTPUT_FILE.open(FName, std::ios::out);      
      OUTPUT_FILE << "||||" << "\n";    
    }

    bool runOnFunction(Function &F) {
      OUTPUT_FILE << "~~~~" << "\n";    

      // Define used function
      LLVMContext &Ctx = F.getContext();

      //for (auto &B : F) {
      for (BasicBlock &BB : F) {
        OUTPUT_FILE << "666" << "\n";    

        bool do_inst = false;
        // Instruction* inst = BB->getFirstNonPHI();
        // BasicBlock::iterator it(inst);
        //++it; //skip first load
        //++it; //skip second load

        //https://www.inf.ed.ac.uk/teaching/courses/ct/17-18/slides/llvm-2-writing_pass.pdf
        for (Instruction &I : BB) {
          OUTPUT_FILE << "777" << "\n";    
          if (BranchInst *BI = dyn_cast<BranchInst>(&I)) {
            OUTPUT_FILE << "888" << "\n";    

            if(BI->isConditional() && isa<CmpInst>(BI->getCondition())) {
              CmpInst * CI = dyn_cast<CmpInst>(BI->getCondition());

              Constant *currentConstantFunc = NULL;

              auto *opA = CI->getOperand(0);
              auto *opB = CI->getOperand(1);
              auto argsA = llvm::ArrayRef<llvm::Value *>(&opA, 1);
              auto argsB = llvm::ArrayRef<llvm::Value *>(&opB, 1);
            
              //assuming both operands are of the same type for conditional
              OUTPUT_FILE << "===" << "\n"; 
              if (opA->getType()->isDoubleTy()) {
                OUTPUT_FILE << "double" << "\n"; 
              } else if (opA->getType()->isIntegerTy(32)) {
                OUTPUT_FILE << "integer" << "\n"; 
              } else if (opA->getType()->isFloatTy()) {
                OUTPUT_FILE << "float" << "\n"; 
              } //add more type distinctions here such as ptr
            }          
          } 
        }
      }
      OUTPUT_FILE.close();
      return false;
    }
  };

  char CRCPass::ID = 0;
  static RegisterPass<CRCPass> X("crc-pass", "Modify equality conditions with CRC",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
}