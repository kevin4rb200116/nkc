#include "nkc/common.hh"
#include "nkc/tokenize/main.hh"
#include "nkc/codegen.hh"

#ifndef ParserHeader
#define ParserHeader

namespace nkc::parse {
  using tokenize::Tokenizer;
  using tokenize::Token;

  extern map<string,int> binop_precedence;

  typedef enum : uint8_t {
    Error,
    Ignore,
    Definition,
    TopLevelExpression,
    Extern,
  } Result;

  typedef struct Parser {
    Tokenizer tokenize;
    struct {
      unique_ptr<codegen::ast::Function> Definition;
      unique_ptr<codegen::ast::Function> TopLevelExpression;
      unique_ptr<codegen::ast::Prototype> Extern;
    } result;

    int get_token_precedence();

    unique_ptr<codegen::ast::Node> Paren();
    unique_ptr<codegen::ast::Node> Number();
    unique_ptr<codegen::ast::Node> Identifier();

    unique_ptr<codegen::ast::Node> Primary();
    unique_ptr<codegen::ast::Node>
      RHS(int expr_prec, unique_ptr<codegen::ast::Node> lhs);
    unique_ptr<codegen::ast::Node> LHS();
    unique_ptr<codegen::ast::Prototype> Prototype();

    unique_ptr<codegen::ast::Function> Definition();
    unique_ptr<codegen::ast::Function> TopLevelExpression();
    unique_ptr<codegen::ast::Prototype> Extern();

    Parser(FILE* fp)
      : tokenize(Tokenizer(fp)) {}
    
    Result next();
  } Parser;
} // namespace nkc::parse

#endif //ParserHeader