#include "token/token.hpp"

std::string token_type_to_str(Token_Type tt) {
#define ENUM_TO_STR(p) \
  case (p):            \
    return std::string(#p)

  switch (tt) {
    ENUM_TO_STR(Token_Type::TT_LEFT_PAREN);
    ENUM_TO_STR(Token_Type::TT_RIGHT_PAREN);
    ENUM_TO_STR(Token_Type::TT_LEFT_BRACE);
    ENUM_TO_STR(Token_Type::TT_RIGHT_BRACE);
    ENUM_TO_STR(Token_Type::TT_COMMA);
    ENUM_TO_STR(Token_Type::TT_DOT);
    ENUM_TO_STR(Token_Type::TT_MINUS);
    ENUM_TO_STR(Token_Type::TT_PLUS);
    ENUM_TO_STR(Token_Type::TT_SEMICOLON);
    ENUM_TO_STR(Token_Type::TT_SLASH);
    ENUM_TO_STR(Token_Type::TT_STAR);
    ENUM_TO_STR(Token_Type::TT_BANG);
    ENUM_TO_STR(Token_Type::TT_BANG_EQUAL);
    ENUM_TO_STR(Token_Type::TT_EQUAL);
    ENUM_TO_STR(Token_Type::TT_EQUAL_EQUAL);
    ENUM_TO_STR(Token_Type::TT_GREATER);
    ENUM_TO_STR(Token_Type::TT_GREATER_EQUAL);
    ENUM_TO_STR(Token_Type::TT_LESS);
    ENUM_TO_STR(Token_Type::TT_LESS_EQUAL);
    ENUM_TO_STR(Token_Type::TT_IDENTIFIER);
    ENUM_TO_STR(Token_Type::TT_STRING);
    ENUM_TO_STR(Token_Type::TT_NUMBER);
    ENUM_TO_STR(Token_Type::TT_AND);
    ENUM_TO_STR(Token_Type::TT_CLASS);
    ENUM_TO_STR(Token_Type::TT_ELSE);
    ENUM_TO_STR(Token_Type::TT_FALSE);
    ENUM_TO_STR(Token_Type::TT_FUN);
    ENUM_TO_STR(Token_Type::TT_FOR);
    ENUM_TO_STR(Token_Type::TT_IF);
    ENUM_TO_STR(Token_Type::TT_NIL);
    ENUM_TO_STR(Token_Type::TT_OR);
    ENUM_TO_STR(Token_Type::TT_PRINT);
    ENUM_TO_STR(Token_Type::TT_RETURN);
    ENUM_TO_STR(Token_Type::TT_SUPER);
    ENUM_TO_STR(Token_Type::TT_THIS);
    ENUM_TO_STR(Token_Type::TT_TRUE);
    ENUM_TO_STR(Token_Type::TT_VAR);
    ENUM_TO_STR(Token_Type::TT_WHILE);
    ENUM_TO_STR(Token_Type::TT_EOF);
  }

  return std::string("");
}

Token::Token(Token_Type type, std::string& lexeme, std::any& literal,
             uint64_t line) {
  this->type = type;
  this->lexeme = lexeme;
  this->literal = literal;
  this->line = line;
}

std::string Token::to_string() const {
  std::string s =
      "type: " + token_type_to_str(this->type) + "lexeme: " + this->lexeme;
  return s;
}
