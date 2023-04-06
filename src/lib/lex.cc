#include "nkc/lex.hh"

#define TokenEntry(name) {Token::name, #name}

namespace nkc::lex {
  map<int16_t,string> token_name = {
    TokenEntry(EndMarker),
    TokenEntry(Definition),
    TokenEntry(Extern),
    TokenEntry(Identifier),
    TokenEntry(Number),
  };
}