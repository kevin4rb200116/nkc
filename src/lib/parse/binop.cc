#include "nkc/parse.hh"

namespace nkc::parse {
  map<string,int> binop_precedence = {
    {"<",10},
    {"+",20},
    {"-",10},
    {"*",40},
  };

  int Parser::get_token_precedence() {
    if (tokenize.current.type == ntokenize::lex::Token::Name)
      return -1;

    int token_precedence = binop_precedence[tokenize.current.value.raw];

    if (token_precedence <= 0)
      return -1;

    return token_precedence;
  }
} // namespace nkc::parse
