#include "nkc/common.hh"
#include "nkc/tokenize/main.hh"
#include "nkc/tokenize/ast.hh"

#ifndef ParserHeader
#define ParserHeader

namespace nkc::parse {
  using tokenize::Tokenizer;
  using tokenize::Token;

  extern map<string,int> binop_precedence;

  typedef struct Parser {
    Tokenizer tokenize;
    Codegen code;

    int get_token_precedence();

    unique_ptr<tokenize::ast::Expression> Paren();
    unique_ptr<tokenize::ast::Expression> Number();
    unique_ptr<tokenize::ast::Expression> Identifier();

    unique_ptr<tokenize::ast::Expression> Primary();
    unique_ptr<tokenize::ast::Expression>
      RHS(int expr_prec, unique_ptr<tokenize::ast::Expression> lhs);
    unique_ptr<tokenize::ast::Expression> LHS();
    unique_ptr<tokenize::ast::Prototype> Prototype();

    unique_ptr<tokenize::ast::Function> Definition();
    unique_ptr<tokenize::ast::Function> TopLevelExpression();
    unique_ptr<tokenize::ast::Prototype> Extern();

    void next();

    Parser(FILE* fp)
      : tokenize(Tokenizer(fp)) {}
  } Parser;
} // namespace nkc::parse

#endif //ParserHeader