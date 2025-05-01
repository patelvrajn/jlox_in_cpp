#pragma once

#include <stdint.h>

#include <any>
#include <string>

enum class Token_Type {
  TT_LEFT_PAREN,
  TT_RIGHT_PAREN,
  TT_LEFT_BRACE,
  TT_RIGHT_BRACE,
  TT_COMMA,
  TT_DOT,
  TT_MINUS,
  TT_PLUS,
  TT_SEMICOLON,
  TT_SLASH,
  TT_STAR,
  TT_BANG,
  TT_BANG_EQUAL,
  TT_EQUAL,
  TT_EQUAL_EQUAL,
  TT_GREATER,
  TT_GREATER_EQUAL,
  TT_LESS,
  TT_LESS_EQUAL,
  TT_IDENTIFIER,
  TT_STRING,
  TT_NUMBER,
  TT_AND,
  TT_CLASS,
  TT_ELSE,
  TT_FALSE,
  TT_FUN,
  TT_FOR,
  TT_IF,
  TT_NIL,
  TT_OR,
  TT_PRINT,
  TT_RETURN,
  TT_SUPER,
  TT_THIS,
  TT_TRUE,
  TT_VAR,
  TT_WHILE,
  TT_EOF
};

std::string token_type_to_str(Token_Type tt);

class Token {
 public:
  Token(Token_Type type, std::string& lexeme, std::any& literal, uint64_t line);

  std::string to_string() const;

 private:
  Token_Type type;
  std::string lexeme;
  std::any literal;
  uint64_t line;
};
