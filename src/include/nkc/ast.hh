#include "common.hh"

#ifndef ASTHeader
#define ASTHeader

namespace nkc::ast {
  typedef struct Expression {
    virtual ~Expression() = default;
    virtual llvm::Value* codegen(Codegen* code) = 0;
  } Expression;

  typedef struct Number:Expression {
    double value;

    Number(double value)
      : value(value) {}
    
    llvm::Value* codegen(Codegen* code) override;
  } Number;

  typedef struct Variable:Expression {
    string name;

    Variable(const string name)
      : name(name) {}

    llvm::Value* codegen(Codegen* code) override;
  } Variable;

  typedef struct Binary:Expression {
    string op;
    unique_ptr<Expression> lhs, rhs;
  
    Binary(string op, unique_ptr<Expression> lhs, unique_ptr<Expression> rhs)
      : op(op), lhs(move(lhs)), rhs(move(rhs)) {}

    llvm::Value* codegen(Codegen* code) override;
  } Binary;

  typedef struct Call:Expression {
    string callee;
    vector<unique_ptr<Expression>> args;

    Call(const string callee, vector<unique_ptr<Expression>> args)
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
    unique_ptr<Expression> body;

    Function(unique_ptr<Prototype> proto, unique_ptr<Expression> body)
      : proto(move(proto)), body(move(body)) {}

    llvm::Function* codegen(Codegen* code);
  } Function;
} // namespace nkc::ast


#endif //ASTHeader