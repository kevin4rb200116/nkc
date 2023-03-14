#include "common.hh"
#include "lex.hh"

#ifndef NKCTokenizeHeader
#define NKCTokenizeHeader

namespace nkc::tokenize {
  using namespace std;
  using ntokenize::TokenInfo;

  typedef struct Tokenizer {
    private:
      ntokenize::Tokenizer tokenize;

    public:
      Tokenizer(FILE* fp)
        : tokenize(ntokenize::Tokenizer(fp)) {}

      TokenInfo* next();
  } Tokenizer;
} // namespace nkc::tokenize

#endif //NKCTokenizeHeader