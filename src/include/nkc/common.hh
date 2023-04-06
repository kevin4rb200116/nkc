#ifndef NKCCommonHeader
#define NKCCommonHeader

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include "ntokenize/main.hh"

namespace nkc {
  using namespace std;

  typedef struct Codegen {
    unique_ptr<llvm::LLVMContext> context;
    unique_ptr<llvm::Module> module;
    unique_ptr<llvm::IRBuilder<>> builder;
    map<string, llvm::Value*> named_values;

    Codegen()
      : context(make_unique<llvm::LLVMContext>()),
        module(make_unique<llvm::Module>("nkc-jit",*context)),
        builder(make_unique<llvm::IRBuilder<>>(*context)) {}
  } Codegen;
} // namespace nkc

#endif //NKCCommonHeader
