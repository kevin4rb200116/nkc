#include "nkc/common.hh"

#ifndef LEX_H
#define LEX_H

namespace nkc::tokenize::lex {
  typedef enum Token : int16_t {
    EndMarker = 257,
    Definition,
    Extern,
    Identifier,
    Number,
  } Token;

  extern map<int16_t,string> token_name;
} // namespace nkc::tokenize::lex

#endif //LEX_H