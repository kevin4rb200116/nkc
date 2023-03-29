#include "nkc/tokenize.hh"

namespace nkc::tokenize {
  Token* Tokenizer::next() {
    ntokenize::Tokenizer::next();

    while (true) {
      if(current.type == ntokenize::lex::Token::Ignore) {
        ntokenize::Tokenizer::next();;
        continue;
      }

      if(current.type == ntokenize::lex::Token::NewLine) {
        ntokenize::Tokenizer::next();;
        continue;
      }

      if(current.type == ntokenize::lex::Token::Comment) {
        ntokenize::Tokenizer::next();
        continue;
      }

      if(current.type == ntokenize::lex::Token::Error) {
        return nullptr;
      }

      break;
    }

    if (current.type == ntokenize::lex::Name) {
      if (current.value == "def")
        current.type = lex::Token::Definition;
      else if (current.value == "extern")
        current.type = lex::Token::Extern;
      else
        current.type = lex::Token::Identifier;
    }

    if (current.type == ntokenize::lex::Token::Number)
      current.type = lex::Token::Number;

    if (current.type == ntokenize::lex::Token::EndMarker)
      current.type = lex::Token::EndMarker;

    return &last;
  }
} // namespace nkc::tokenize
