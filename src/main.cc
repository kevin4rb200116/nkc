#include "nkc/parser.hh"
#include <memory>

using namespace nkc::parser;

int main(int argc, char** argv, char** environ) {
  Parser parse = Parser(stdin);

  while (!feof(stdin)) {
    if (parse.tokenize.current.type == nkc::lex::Token::EndMarker)
      break;

    parse.next();
  }

  return EXIT_SUCCESS;
}