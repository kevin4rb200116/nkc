#include "common.hh"
#include "lex.hh"

#ifndef NKCTokenizeHeader
#define NKCTokenizeHeader

namespace nkc::tokenize {
  using namespace std;
  using ntokenize::Token;

  typedef struct Tokenizer:ntokenize::Tokenizer {
    Tokenizer(FILE* fp)
      : ntokenize::Tokenizer(fp) {}

    Token* next();
  } Tokenizer;
} // namespace nkc::tokenize

#endif //NKCTokenizeHeader