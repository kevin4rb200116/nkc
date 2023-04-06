#include "nkc/parser.hh"

namespace nkc::parser {
  unique_ptr<ast::Expression> Parser::Paren() {
    tokenize.next(); // eat (.

    auto v = LHS();

    if (!v)
      return nullptr;

    if (tokenize.current.value != ")") {
      fprintf(stderr,"expected ')'");
      return nullptr;
    }

    tokenize.next(); // eat ).

    return v;
  }

  unique_ptr<ast::Expression> Parser::Number() {
    auto result = make_unique<ast::Number>(strtod(tokenize.current.value.c_str(),nullptr));
    tokenize.next();

    return move(result);
  }

  unique_ptr<ast::Expression> Parser::Identifier() {
    string name = string(tokenize.current.value);
    vector<unique_ptr<ast::Expression>> args;

    tokenize.next(); // eat identifier.

    if (tokenize.current.value != "(") {
      return make_unique<ast::Variable>(name);
    } else if (tokenize.current.value == "(") {
      for (tokenize.next();; tokenize.next()) {
        if (auto arg = LHS())
          args.push_back(move(arg));
        else
          return nullptr;

        if (tokenize.current.value == ")")
          break;
        else if (tokenize.current.value != ",") {
          fprintf(stderr,"Expected ')' or ',' in argument list\n");
          return nullptr;
        }
      }

      tokenize.next(); // Eat the ')'.
    }

    return make_unique<ast::Call>(name, move(args));
  }

  unique_ptr<ast::Expression> Parser::Primary() {
    unique_ptr<ast::Expression> primary;

    if (tokenize.current.type == lex::Token::Identifier) {
      primary = Identifier();
    } else if (tokenize.current.type == lex::Token::Number) {
      primary = Number();
    } else if (tokenize.current.value == "(") {
      primary = Paren();
    } else {
      fprintf(stderr,"unknown token when expecting an expression\n");
      primary = nullptr;
    }

    return primary;
  }

  unique_ptr<ast::Expression> Parser::RHS(int expr_prec,
                                    unique_ptr<ast::Expression> lhs) {
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

      lhs = make_unique<ast::Binary>(bin_op.value,move(lhs),move(rhs));
    }
  }

  unique_ptr<ast::Expression> Parser::LHS() {
    auto lhs = Primary();

    if (!lhs)
      return nullptr;

    return RHS(0, move(lhs));
  }

  unique_ptr<ast::Prototype> Parser::Prototype() {
    if (tokenize.current.type != lex::Token::Identifier) {
      fprintf(stderr, "Expected function name in prototype\n");
      return nullptr;
    }

    string fname = string(tokenize.current.value);

    tokenize.next();

    if (tokenize.current.value != "(")
      return nullptr;

    vector<string> arg_names;

    for (tokenize.next();; tokenize.next())
      if (tokenize.current.type != lex::Identifier)
        break;
      else
        arg_names.push_back(string(tokenize.current.value));

    if (tokenize.current.value != ")")
      return nullptr;

    tokenize.next();

    return make_unique<ast::Prototype>(fname,move(arg_names));
  }

  unique_ptr<ast::Function> Parser::Definition() {
    tokenize.next(); // eat def.

    auto proto = Prototype();

    if (!proto)
      return nullptr;

    if (auto e = LHS())
      return make_unique<ast::Function>(move(proto), move(e));

    return nullptr;
  }

  unique_ptr<ast::Function> Parser::TopLevelExpression() {
    unique_ptr<ast::Function> expr;

    if (auto e = LHS()) {
      auto proto = make_unique<ast::Prototype>("__anon_expr", vector<string>());

      expr = make_unique<ast::Function>(move(proto), move(e));
    } else {
      expr = nullptr;
    }

    return expr;
  }

  unique_ptr<ast::Prototype> Parser::Extern() {
    tokenize.next(); // eat extern.
    auto proto = Prototype();

    return proto;
  }

  void Parser::next() {
    fprintf(stderr, "> ");
    tokenize.next();

    if (tokenize.current.value == ";") {
      return;
    } else if (tokenize.current.type == lex::Token::Definition) {
      auto ast = Definition();

      if (ast) {
        if (auto* function = ast->codegen(&code)) {
          fprintf(stderr, "Read function definition:\n");
          function->print(llvm::errs());
          fprintf(stderr,"\n");
        }
      } else
        fprintf(stderr, "error while parsing definition.\n");

      return;
    } else if (tokenize.current.type == lex::Token::Extern) {
      auto ast = Extern();

      if (ast) {
        if (auto* function=ast->codegen(&code)) {
          fprintf(stderr, "Read extern:\n");
          function->print(llvm::errs());
          fprintf(stderr,"\n");
        }
      } else
        fprintf(stderr, "error while parsing extern\n");

      return;
    } else {
      auto ast=TopLevelExpression();

      if (ast) {
        if (auto* function=ast->codegen(&code)) {
          fprintf(stderr, "Read top-level expression:\n");
          function->print(llvm::errs());
          fprintf(stderr,"\n");

          function->removeFromParent();
        }
      } else
        fprintf(stderr, "error while parsing top-level expression");

      return;
    }
  }
} // namespace nkc::parser
