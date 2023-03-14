#include "nkc/tokenize.hh"

namespace nkc::tokenize {
  TokenInfo* Tokenizer::next() {
    TokenInfo* token = tokenize.next();

    while (true) {
      if(token->type == ntokenize::lex::token["TYPE_IGNORE"]) {
        token = tokenize.next();
        continue;
      }

      if(token->type == ntokenize::lex::token["NL"]) {
        token = tokenize.next();
        continue;
      }

      if(token->type == ntokenize::lex::token["COMMENT"]) {
        token = tokenize.next();
        continue;
      }

      break;
    }

    if (token->type == ntokenize::lex::token["NAME"]) {
      if (token->raw_value == "def")
        token->type = lex::token["DEF"];
      else if (token->raw_value == "extern")
        token->type = lex::token["EXTERN"];
      else
        token->type = lex::token["IDENTIFIER"];
    }

    if (token->type == ntokenize::lex::token["NUMBER"])
      token->type = lex::token["NUMBER"];

    if (token->type == ntokenize::lex::token["ENDMARKER"])
      token->type = lex::token["EOF"];

    return token;
  }
} // namespace nkc::tokenize
