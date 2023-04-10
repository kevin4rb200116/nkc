#include "nkc/parse.hh"
#include <memory>

using namespace nkc::parse;

int main(int argc, char** argv, char** environ) {
  FILE* inputf;

  if (argc == 2) {
    inputf = fopen(argv[1],"r");

    if (!inputf) {
      fprintf(stderr, "error trying to open file: %s", argv[1]);
      return EXIT_FAILURE;
    }
  } else {
    inputf = stdin;
  }

  Parser parse = Parser(inputf);

  while (!feof(stdin)) {
    if (parse.tokenize.current.type == nkc::tokenize::lex::Token::EndMarker)
      break;

    parse.next();
  }

  parse.code.module->print(llvm::errs(), nullptr);

  return EXIT_SUCCESS;
}