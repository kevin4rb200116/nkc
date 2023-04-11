#include "nkc/main.hh"

namespace nkc {
  void Driver::next() {
    fprintf(stderr, "> ");
    auto r = parse.next();

    if (r == parse::Result::Error) {
      exit(EXIT_FAILURE);
    } else if (r == parse::Result::Ignore) {
      return;
    } else if (r == parse::Result::Definition) {
      auto def_node = move(parse.result.Definition);

      if (auto* function = def_node->codegen(&code)) {
        fprintf(stderr, "Read function definition:\n");
        function->print(llvm::errs());
        fprintf(stderr,"\n");
      }
    } else if (r == parse::Result::Extern) {
      auto extern_node = move(parse.result.Extern);

      if (auto* function=extern_node->codegen(&code)) {
        fprintf(stderr, "Read extern:\n");
        function->print(llvm::errs());
        fprintf(stderr,"\n");
      }
    } else if (r == parse::Result::TopLevelExpression) {
      auto expr_node = move(parse.result.TopLevelExpression);

      if (auto* function=expr_node->codegen(&code)) {
        fprintf(stderr, "Read top-level expression:\n");
        function->print(llvm::errs());
        fprintf(stderr,"\n");
      }
    }
  }
} // namespace nkc