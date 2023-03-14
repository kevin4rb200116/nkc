#include "nkc/tokenize.hh"
#include <string>

using namespace std;
using nkc::tokenize::Tokenizer;

int main(int argc, char* argv[]) {
  Tokenizer tokenize = Tokenizer(stdin);

  for (auto token=tokenize.next();
       token->type != nkc::lex::token["EOF"];
       token=tokenize.next()) {

    if (token->type == ntokenize::lex::token["ERRORTOKEN"])
      return EXIT_FAILURE;

    string type = nkc::lex::token_name[token->type];

    if (type.empty())
      printf("\"%s\"\n", token->raw_value.c_str());
    else
      printf("%s: %s\n", type.c_str(), token->raw_value.c_str());
  }

  return EXIT_SUCCESS;
}
