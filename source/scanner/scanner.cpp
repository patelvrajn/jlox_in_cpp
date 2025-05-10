#include "scanner/scanner.hpp"

#include "token/token.hpp"

Scanner::Scanner(const std::string& source, Error_Reporter& e) {
  this->source = source;
  this->error_reporting = e;
  tokens = new Dynamic_Array<Token>();

  this->start = 0;  // Index of the first character in the lexeme.
  this->current =
      0;  // Index of the current character being considered in the lexeme.
  this->line = 1;  // Current line number of source code.

  this->lox_keywords = new Hash_Table<std::string, Token_Type>();

#define ENUM_TO_STR(v) std::string(#v).substr(3), v

  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_AND));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_CLASS));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_ELSE));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_FALSE));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_FOR));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_FUN));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_IF));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_NIL));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_OR));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_PRINT));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_RETURN));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_SUPER));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_THIS));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_TRUE));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_VAR));
  this->lox_keywords->insert(ENUM_TO_STR(Token_Type::TT_WHILE));

#undef ENUM_TO_STR
}

bool Scanner::is_at_end() const { return (current >= source.length()); }

// Returns the current character and advances current.
char Scanner::advance() {
  current++;
  return source[(current - 1)];
}

void Scanner::add_token(Token_Type type) { add_token(type, NULL); }

void Scanner::add_token(Token_Type type, const std::any& literal) {
  std::string text = source.substr(start, (current - start));
  Token t(type, text, literal, line);
  tokens->insert(t, (tokens->get_maximum_index() + 1));
}

Dynamic_Array<Token>& Scanner::scan_tokens() {
  // Scan tokens until end of source.
  while (!is_at_end()) {
    start = current;  // We are at the beginning of the next lexeme.
    scan_token();
  }

  // Add end of file (EOF) token.
  Token t(Token_Type::TT_EOF, "", NULL, line);
  tokens->insert(t, (tokens->get_maximum_index() + 1));

  return *tokens;
}

void Scanner::scan_token() {
  char c = advance();

  switch (c) {
    case '(':
      add_token(Token_Type::TT_LEFT_PAREN);
      break;
    case ')':
      add_token(Token_Type::TT_RIGHT_PAREN);
      break;
    case '{':
      add_token(Token_Type::TT_LEFT_BRACE);
      break;
    case '}':
      add_token(Token_Type::TT_RIGHT_BRACE);
      break;
    case ',':
      add_token(Token_Type::TT_COMMA);
      break;
    case '.':
      add_token(Token_Type::TT_DOT);
      break;
    case '-':
      add_token(Token_Type::TT_MINUS);
      break;
    case '+':
      add_token(Token_Type::TT_PLUS);
      break;
    case ';':
      add_token(Token_Type::TT_SEMICOLON);
      break;
    case '*':
      add_token(Token_Type::TT_STAR);
      break;
    case '!':
      add_token(match('=') ? Token_Type::TT_BANG_EQUAL : Token_Type::TT_BANG);
      break;
    case '=':
      add_token(match('=') ? Token_Type::TT_EQUAL_EQUAL : Token_Type::TT_EQUAL);
      break;
    case '<':
      add_token(match('=') ? Token_Type::TT_LESS_EQUAL : Token_Type::TT_LESS);
      break;
    case '>':
      add_token(match('=') ? Token_Type::TT_GREATER_EQUAL
                           : Token_Type::TT_GREATER);
      break;
    case '/':
      if (match('/')) {
        // Ignore comments.
        while ((peek() != '\n') && (!is_at_end())) {
          advance();
        }
      } else {
        add_token(Token_Type::TT_SLASH);
      }
      break;
    case ' ':
      break;
    case '\r':
      break;
    case '\t':
      break;
    case '\n':
      line++;
      break;
    case '"':
      lox_string();
      break;
    default:
      if (is_digit(c)) {
        lox_number();
      } else if (is_alpha(c)) {
        lox_identifier();
      } else {
        error_reporting.error(line, "Unexpected character.");
      }
      return;
  }
}

void Scanner::lox_identifier() {
  while (is_alpha_numeric(peek())) {
    advance();
  }
  std::string text = source.substr(start, (current - start));
  Token_Type* type = lox_keywords->search(text);
  if (type == nullptr) {
    *type = Token_Type::TT_IDENTIFIER;
  }
  add_token(*type);
}

void Scanner::lox_number() {
  while (is_digit(peek())) {
    advance();
  }

  // Look for the fractional part.
  if ((peek() == '.') && (is_digit(peek_next()))) {
    // Consume the ".".
    advance();

    while (is_digit(peek())) {
      advance();
    }
  }

  add_token(Token_Type::TT_NUMBER,
            std::stod(source.substr(start, (current - start))));
}

void Scanner::lox_string() {
  while ((peek() != '"') && (!is_at_end())) {
    // Lox supports multiline strings but doesn't support escape sequences which
    // is why this logic works.
    if (peek() == '\n') {
      line++;
    }

    advance();
  }

  if (is_at_end()) {
    error_reporting.error(line, "Unterminated string.");
    return;
  }

  // The closing ".
  advance();

  // Trim the surrounding quotes and add a string token.
  std::string value = source.substr((start + 1), (current - start - 2));
  add_token(Token_Type::TT_STRING, value);
}

/* A conditional advance; only advance current if the current character is the
expected character. */
bool Scanner::match(char expected) {
  if (is_at_end()) {
    return false;
  }

  if (source[current] != expected) {
    return false;
  }

  current++;
  return true;
}

char Scanner::peek() {
  if (is_at_end()) {
    return '\0';
  }

  return source[current];
}

char Scanner::peek_next() {
  if ((current + 1) >= source.length()) {
    return '\0';
  }

  return source[current + 1];
}

bool Scanner::is_alpha(char c) {
  return (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||
          (c == '_'));
}

bool Scanner::is_alpha_numeric(char c) { return (is_alpha(c) || is_digit(c)); }

bool Scanner::is_digit(char c) { return ((c >= '0') && (c <= '9')); }
