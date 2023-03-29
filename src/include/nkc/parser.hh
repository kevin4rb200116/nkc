#include "common.hh"
#include "tokenize.hh"
#include "nkc/ast.hh"

#ifndef ParserHeader
#define ParserHeader

namespace nkc::parser {
  using tokenize::Tokenizer;
  using tokenize::Token;

  extern map<string,int> binop_precedence;

  typedef struct Parser {
    Tokenizer tokenize;

    int get_token_precedence();

    unique_ptr<ast::Expression> Paren();
    unique_ptr<ast::Expression> Number();
    unique_ptr<ast::Expression> Identifier();

    unique_ptr<ast::Expression> Primary();
    unique_ptr<ast::Expression> RHS(int expr_prec,
                                    unique_ptr<ast::Expression> lhs);
    unique_ptr<ast::Expression> LHS();
    unique_ptr<ast::Prototype> Prototype();

    unique_ptr<ast::Function> Definition();
    unique_ptr<ast::Function> TopLevelExpression();
    unique_ptr<ast::Prototype> Extern();

    void next();

    Parser(FILE* fp)
      : tokenize(Tokenizer(fp)) {}
  } Parser;
} // namespace nkc::parser


#endif //ParserHeader