#include "nkc/parse.hh"

namespace nkc::parse {
  unique_ptr<codegen::ast::Node> Parser::Paren() {
    tokenize.next(); // eat (.

    auto v = LHS();

    if (!v)
      return nullptr;

    if (tokenize.current.value.raw != ")") {
      fprintf(stderr,"expected ')'");
      return nullptr;
    }

    tokenize.next(); // eat ).

    return v;
  }

  unique_ptr<codegen::ast::Node> Parser::Number() {
    auto result = make_unique<codegen::ast::Number>(strtod(tokenize.current.value.raw.c_str(),nullptr));
    tokenize.next();

    return move(result);
  }

  unique_ptr<codegen::ast::Node> Parser::Identifier() {
    string name = string(tokenize.current.value.raw);
    vector<unique_ptr<codegen::ast::Node>> args;

    tokenize.next(); // eat identifier.

    if (tokenize.current.value.raw != "(") {
      return make_unique<codegen::ast::Variable>(name);
    } else if (tokenize.current.value.raw == "(") {
      for (tokenize.next();; tokenize.next()) {
        if (auto arg = LHS())
          args.push_back(move(arg));
        else
          return nullptr;

        if (tokenize.current.value.raw == ")")
          break;
        else if (tokenize.current.value.raw != ",") {
          fprintf(stderr,"Expected ')' or ',' in argument list\n");
          return nullptr;
        }
      }

      tokenize.next(); // Eat the ')'.
    }

    return make_unique<codegen::ast::Call>(name, move(args));
  }

  unique_ptr<codegen::ast::Node> Parser::Primary() {
    unique_ptr<codegen::ast::Node> primary;

    if (tokenize.current.type == tokenize::lex::Token::Identifier) {
      primary = Identifier();
    } else if (tokenize.current.type == tokenize::lex::Token::Number) {
      primary = Number();
    } else if (tokenize.current.value.raw == "(") {
      primary = Paren();
    } else {
      fprintf(stderr,"unknown token when expecting an expression\n");
      primary = nullptr;
    }

    return primary;
  }

  unique_ptr<codegen::ast::Node> Parser::RHS(int expr_prec,
                                    unique_ptr<codegen::ast::Node> lhs) {
    while (true) {
      int token_precedence = get_token_precedence();

      if (token_precedence < expr_prec)
        return lhs;

      Token bin_op;
      bin_op.copy(&tokenize.current);

      tokenize.next();

      auto rhs = Primary();

      if (!rhs)
        return nullptr;

      int next_precedence = get_token_precedence();

      if (token_precedence < next_precedence) {
        rhs = RHS(token_precedence+1, move(rhs));

        if (!rhs)
          return nullptr;
      }

      lhs = make_unique<codegen::ast::Binary>(bin_op.value.raw, move(lhs),move(rhs));
    }
  }

  unique_ptr<codegen::ast::Node> Parser::LHS() {
    auto lhs = Primary();

    if (!lhs)
      return nullptr;

    return RHS(0, move(lhs));
  }

  unique_ptr<codegen::ast::Prototype> Parser::Prototype() {
    if (tokenize.current.type != tokenize::lex::Token::Identifier) {
      fprintf(stderr, "Expected function name in prototype\n");
      return nullptr;
    }

    string fname = string(tokenize.current.value.raw);

    tokenize.next();

    if (tokenize.current.value.raw != "(")
      return nullptr;

    vector<string> arg_names;

    for (tokenize.next();; tokenize.next())
      if (tokenize.current.type != tokenize::lex::Identifier)
        break;
      else
        arg_names.push_back(string(tokenize.current.value.raw));

    if (tokenize.current.value.raw != ")")
      return nullptr;

    tokenize.next();

    return make_unique<codegen::ast::Prototype>(fname,move(arg_names));
  }

  unique_ptr<codegen::ast::Function> Parser::Definition() {
    tokenize.next(); // eat def.

    auto proto = Prototype();

    if (!proto)
      return nullptr;

    if (auto e = LHS())
      return make_unique<codegen::ast::Function>(move(proto), move(e));

    return nullptr;
  }

  unique_ptr<codegen::ast::Function> Parser::TopLevelExpression() {
    unique_ptr<codegen::ast::Function> expr;

    if (auto e = LHS()) {
      auto proto = make_unique<codegen::ast::Prototype>("__anon_expr", vector<string>());

      expr = make_unique<codegen::ast::Function>(move(proto), move(e));
    } else {
      expr = nullptr;
    }

    return expr;
  }

  unique_ptr<codegen::ast::Prototype> Parser::Extern() {
    tokenize.next(); // eat extern.
    auto proto = Prototype();

    return proto;
  }

  Result Parser::next() {
    tokenize.next();

    if (tokenize.current.value.raw == ";") {
      return Result::Ignore;
    } else if (tokenize.current.type == tokenize::lex::Token::Definition) {
      auto ast = Definition();

      if (ast) {
        result.Definition = move(ast);
        return Result::Definition;
      } else
        fprintf(stderr, "error while parsing definition.\n");

      return Result::Error;

    } else if (tokenize.current.type == tokenize::lex::Token::Extern) {
      auto ast = Extern();

      if (ast) {
        result.Extern = move(ast);
        return Result::Extern;
      } else
        fprintf(stderr, "error while parsing extern\n");

      return Result::Error;

    } else {
      auto ast=TopLevelExpression();

      if (ast) {
        result.TopLevelExpression = move(ast);
        return Result::TopLevelExpression;
      } else
        fprintf(stderr, "error while parsing top-level expression\n");

      return Result::Error;
    }
  }
} // namespace nkc::parse
