#include "simple_parser/lexer.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>

using namespace Simple;
using Simple::TokenType;

TEST_CASE ("Test Lex Tokens") {
  std::string filename = "tests/simple_source/tokens.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(input);
  std::vector<Token*> expected{
      new SymbolToken("procedure"), new SymbolToken("main"),
      new NumberToken("5"),         new NumberToken("50"),
      new PunctToken("{"),          new PunctToken("}"),
      new PunctToken("("),          new PunctToken(")"),
      new PunctToken("="),          new PunctToken("!="),
      new PunctToken("=="),         new PunctToken("!"),
      new PunctToken("&&"),         new PunctToken("||"),
      new SymbolToken("read"),      new SymbolToken("print"),
      new PunctToken("+"),          new PunctToken("-"),
      new PunctToken("*"),          new PunctToken("/"),
      new PunctToken("%"),          new PunctToken(">"),
      new PunctToken(">="),         new PunctToken("<"),
      new PunctToken("<="),         new EndOfFileToken(),
  };

  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
};

TEST_CASE ("Test Lex Assign Statement") {
  std::string filename = "tests/simple_source/assign.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(input);
  std::vector<Token*> expected{
      new SymbolToken("procedure"), new SymbolToken("main"),
      new PunctToken("{"),          new SymbolToken("i"),
      new PunctToken("="),          new NumberToken("5"),
      new PunctToken(";"),          new PunctToken("}"),
      new EndOfFileToken(),
  };

  // Expect that the vector of tokens are equivalent
  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
}

TEST_CASE ("Test Lex While Statement") {
  std::string filename = "tests/simple_source/while.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(input);
  std::vector<Token*> expected{
      new SymbolToken("procedure"), new SymbolToken("main"),
      new PunctToken("{"),          new SymbolToken("while"),
      new PunctToken("("),          new SymbolToken("i"),
      new PunctToken("=="),         new NumberToken("5"),
      new PunctToken(")"),          new PunctToken("{"),
      new SymbolToken("i"),         new PunctToken("="),
      new SymbolToken("i"),         new PunctToken("+"),
      new NumberToken("1"),         new PunctToken(";"),
      new PunctToken("}"),          new PunctToken("}"),
      new EndOfFileToken(),
  };

  // Expect that the vector of tokens are equivalent
  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
}
