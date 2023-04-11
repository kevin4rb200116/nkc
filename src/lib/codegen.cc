#include "nkc/codegen.hh"

namespace nkc::codegen::ast {
  llvm::Value* Number::codegen(Codegen* code) {
    return llvm::ConstantFP::get(*code->context, llvm::APFloat(value));
  }

  llvm::Value* Variable::codegen(Codegen* code) {
    auto* v = code->named_values[name];

    if (!v) {
      fprintf(stderr, "Unknown variable name");

      return nullptr;
    }

    return v;
  }

  llvm::Value* Binary::codegen(Codegen* code) {
    llvm::Value* l = lhs->codegen(code);
    llvm::Value* r = rhs->codegen(code);

    if (!l || !r)
      return nullptr;

    if (op == "+") {
      return code->builder->CreateFAdd(l,r, "addtmp");
    } else if (op == "-") {
      return code->builder->CreateFSub(l,r, "subtmp");
    } else if (op == "*") {
      return code->builder->CreateFMul(l,r, "multmp");
    } else if (op == "<") {
      l =  code->builder->CreateFCmpULT(l,r, "cmptmp");

      // Convert bool 0/1 to double 0.0 or 1.0
      return code->builder->CreateUIToFP(l,
                                          llvm::Type::getDoubleTy(*code->context),
                                          "booltmp");
    } else {
      fprintf(stderr, "invalid binary operator");
      return nullptr;
    }
  }

  llvm::Value* Call::codegen(Codegen* code) {
    llvm::Function* calleef = code->module->getFunction(callee);

    if (!calleef) {
      fprintf(stderr, "Unknown function referenced");
      return nullptr;
    }

    if (calleef->arg_size() != args.size()) {
      fprintf(stderr, "Incorrect # arguments passed");
      return nullptr;
    }

    std::vector<llvm::Value*> argsv;

    for (unsigned i=0, e=args.size(); i != e; ++i) {
      argsv.push_back(args[i]->codegen(code));
      if (!args.back())
        return nullptr;
    }

    return code->builder->CreateCall(calleef,argsv,"calltmp");
  }

  llvm::Function* Prototype::codegen(Codegen* code) {
    // Make the function type:  double(double,double) etc.
    auto doubles=
      vector<llvm::Type*>(args.size(),
                          llvm::Type::getDoubleTy(*(code->context)));
    llvm::FunctionType* ft=
      llvm::FunctionType::get(llvm::Type::getDoubleTy(*(code->context)),
                              doubles,false);

    llvm::Function* f=llvm::Function::Create(ft,
                                             llvm::Function::ExternalLinkage,
                                             name,
                                             code->module.get());

    unsigned idx = 0;
    for (auto& arg:f->args())
      arg.setName(args[idx++]);

    return f;
  }

  llvm::Function* Function::codegen(Codegen* code) {
    // First, check for an existing function from a previous 'extern' declaration.
    llvm::Function* function=code->module->getFunction(llvm::StringRef(proto->name));

    if (!function)
      function = proto->codegen(code);

    if (!function)
      return nullptr;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock* block=llvm::BasicBlock::Create(*(code->context),
                                                       "entry",
                                                       function);
    code->builder->SetInsertPoint(block);

    // Record the function arguments in the NamedValues map.
    code->named_values.clear();
    for (auto& arg:function->args())
      code->named_values[string(arg.getName())] = &arg;

    if (llvm::Value* ret_val=body->codegen(code)) {
      // Finish off the function.
      code->builder->CreateRet(ret_val);

      // Validate the generated code, checking for consistency.
      llvm::verifyFunction(*function);

      return function;
    }

    // Error reading body, remove function.
    function->eraseFromParent();
    return nullptr;
  }
} // namespace nkc::codegen::ast
