#include "common.hh"

#ifndef ASTHeader
#define ASTHeader

namespace nkc::ast {
  typedef struct Expression {
    virtual ~Expression() = default;
  } Expression;

  typedef struct Number:Expression {
    double value;

    Number(double value)
      : value(value) {}
  } Number;

  typedef struct Variable:Expression {
    string name;

    Variable(const string name)
      : name(name) {}
  } Variable;

  typedef struct Binary:Expression {
    string op;
    unique_ptr<Expression> lhs, rhs;
  
    Binary(string op, unique_ptr<Expression> lhs, unique_ptr<Expression> rhs)
      : op(op), lhs(move(lhs)), rhs(move(rhs)) {}
  } Binary;

  typedef struct Call:Expression {
    string callee;
    vector<unique_ptr<Expression>> args;

    Call(const string callee, vector<unique_ptr<Expression>> args)
      : callee(callee), args(move(args)) {}
  } Call;

  typedef struct Prototype:Expression {
    string name;
    vector<string> args;

    const string getName() const { return name; }

    Prototype(string name, vector<string> args)
      : name(name), args(args) {}
  } Prototype;

  typedef struct Function {
    unique_ptr<Prototype> proto;
    unique_ptr<Expression> body;

    Function(unique_ptr<Prototype> proto, unique_ptr<Expression> body)
      : proto(move(proto)), body(move(body)) {}
  } Function;
} // namespace nkc::ast


#endif //ASTHeader