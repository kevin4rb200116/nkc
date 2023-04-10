#include "nkc/tokenize/main.hh"

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
      if (current.value.raw == "def")
        current.type = tokenize::lex::Token::Definition;
      else if (current.value.raw == "extern")
        current.type = tokenize::lex::Token::Extern;
      else
        current.type = tokenize::lex::Token::Identifier;
    }

    if (current.type == ntokenize::lex::Token::FloatNumber
        || current.type == ntokenize::lex::Token::DecNumber)
      current.type = tokenize::lex::Token::Number;

    if (current.type == ntokenize::lex::Token::EndMarker)
      current.type = tokenize::lex::Token::EndMarker;

    return &last;
  }
} // namespace nkc::tokenize
