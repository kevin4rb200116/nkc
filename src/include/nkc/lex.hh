#include "common.hh"

#ifndef LEX_H
#define LEX_H

namespace nkc::lex {
  using ntokenize::lex::TokenList;
  using ntokenize::lex::TokenNameList;
  using ntokenize::lex::OperatorList;

  extern TokenNameList token_name;
  extern TokenList token;
} // namespace nkc::lex


#endif //LEX_H