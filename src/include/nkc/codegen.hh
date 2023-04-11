#include "nkc/common.hh"

#ifndef CodegenHeader
#define CodegenHeader

namespace nkc::codegen {
  typedef struct Codegen Codegen;

  namespace ast {
    typedef struct Node {
      virtual ~Node() = default;
      virtual llvm::Value* codegen(Codegen* code) = 0;
    } Node;

    typedef struct Number:Node {
      double value;

      Number(double value)
        : value(value) {}
      
      llvm::Value* codegen(Codegen* code) override;
    } Number;

    typedef struct Variable:Node {
      string name;

      Variable(const string name)
        : name(name) {}

      llvm::Value* codegen(Codegen* code) override;
    } Variable;

    typedef struct Binary:Node {
      string op;
      unique_ptr<Node> lhs, rhs;
    
      Binary(string op, unique_ptr<Node> lhs, unique_ptr<Node> rhs)
        : op(op), lhs(move(lhs)), rhs(move(rhs)) {}

      llvm::Value* codegen(Codegen* code) override;
    } Binary;

    typedef struct Call:Node {
      string callee;
      vector<unique_ptr<Node>> args;

      Call(const string callee, vector<unique_ptr<Node>> args)
        : callee(callee), args(move(args)) {}

      llvm::Value* codegen(Codegen* code) override;
    } Call;

    typedef struct Prototype {
      string name;
      vector<string> args;

      Prototype(string name, vector<string> args)
        : name(name), args(args) {}

      llvm::Function* codegen(Codegen* code);
    } Prototype;

    typedef struct Function {
      unique_ptr<Prototype> proto;
      unique_ptr<Node> body;

      Function(unique_ptr<Prototype> proto, unique_ptr<Node> body)
        : proto(move(proto)), body(move(body)) {}

      llvm::Function* codegen(Codegen* code);
    } Function;
  } // namespace ast
  

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
} // namespace nkc::codegen::ast

#endif //CodegenHeader