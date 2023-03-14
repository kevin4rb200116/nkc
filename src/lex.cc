#include "nkc/lex.hh"

namespace nkc::lex {
  TokenNameList token_name = {
    { 0, "EOF"},
    {-1, "DEF"},
    {-2, "EXTERN"},
    {-3, "IDENTIFIER"},
    {-4, "NUMBER"},
  };

  TokenList token = {
    {"EOF",  0},
    {"DEF", -1},
    {"EXTERN", -2},
    {"IDENTIFIER", -3},
    {"NUMBER", -4},
  };
}