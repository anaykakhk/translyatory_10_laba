#include "MyGrammarBaseVisitor.h"
#include "MyGrammarLexer.h"
#include "MyGrammarParser.h"
#include <ANTLRInputStream.h>
#include <antlr4-runtime.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class EvaluatorVisitor : public MyGrammarBaseVisitor {
public:
  antlrcpp::Any visitE(MyGrammarParser::EContext *ctx) override {
    if (ctx->e() == nullptr) {
      return visitT(ctx->t());
    }
    int left = std::any_cast<int>(visitE(ctx->e()));
    int right = std::any_cast<int>(visitT(ctx->t()));
    int result = left | right;
    std::cout << left << " | " << right << " = " << result << std::endl;
    return result;
  }

  antlrcpp::Any visitT(MyGrammarParser::TContext *ctx) override {
    if (ctx->t() == nullptr) {
      return visitD(ctx->d());
    }
    int left = std::any_cast<int>(visitT(ctx->t()));
    int right = std::any_cast<int>(visitD(ctx->d()));
    int result = left & right;
    std::cout << left << " & " << right << " = " << result << std::endl;
    return result;
  }

  antlrcpp::Any visitD(MyGrammarParser::DContext *ctx) override {
    if (ctx->d() == nullptr) {
      return visitH(ctx->h());
    }
    int left = std::any_cast<int>(visitD(ctx->d()));
    int right = std::any_cast<int>(visitH(ctx->h()));
    int result = left ^ right;
    std::cout << left << " ^ " << right << " = " << result << std::endl;
    return result;
  }

  antlrcpp::Any visitH(MyGrammarParser::HContext *ctx) override {
    if (ctx->f() != nullptr) {
      return visitF(ctx->f());
    }
    int value = std::any_cast<int>(visitF(ctx->f()));
    int result = ~value;
    std::cout << "~" << value << " = " << result << std::endl;
    return result;
  }

  antlrcpp::Any visitF(MyGrammarParser::FContext *ctx) override {
    if (ctx->I() != nullptr) {
      int result = std::stoi(ctx->I()->getText());
      std::cout << "Parsed int: " << result << std::endl;
      return result;
    }
    return visitE(ctx->e());
  }
};

int main(int argc, const char *argv[]) {
  std::ifstream stream;
  if (argc > 1) {
    stream.open(argv[1]);
    std::cout << argv[1] << std::endl;
  } else {
    stream.open("../test");
  }
  antlr4::ANTLRInputStream input(stream);

  MyGrammarLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);

  tokens.fill();
  for (auto token : tokens.getTokens()) {
    std::cout << token->toString() << std::endl;
  }

  MyGrammarParser parser(&tokens);

  antlr4::tree::ParseTree *tree = parser.e();

  std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

  EvaluatorVisitor visitor;
  int result = std::any_cast<int>(visitor.visit(tree));
  std::cout << "Result: " << result << std::endl;

  return 0;
}
