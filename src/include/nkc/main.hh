#include "nkc/common.hh"
#include "nkc/codegen.hh"
#include "nkc/parse.hh"

#ifndef MainHeader
#define MainHeader

namespace nkc {
  typedef struct Driver {
    parse::Parser parse;
    codegen::Codegen code;

    Driver(FILE* fp)
      : parse(parse::Parser(fp)) {}

    void next();
  } Driver;
} // namespace nkc

#endif //MainHeader