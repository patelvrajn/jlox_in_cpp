#include <string>

#include "data_structures/dynamic_array.hpp"
#include "data_structures/hash_table.hpp"
#include "error_reporter/error_reporter.hpp"
#include "token/token.hpp"

class Scanner {
 public:
  Scanner(const std::string& source, Error_Reporter& e);
  Dynamic_Array<Token>& scan_tokens();

 private:
  std::string source;
  Dynamic_Array<Token>* tokens;
  Hash_Table<std::string, Token_Type>* lox_keywords;

  uint64_t start;
  uint64_t current;
  uint64_t line;

  Error_Reporter error_reporting;

  bool is_at_end() const;
  char advance();
  void add_token(Token_Type type);
  void add_token(Token_Type type, const std::any& literal);
  void scan_token();
  void lox_identifier();
  void lox_number();
  void lox_string();
  bool match(char expected);
  char peek();
  char peek_next();
  bool is_alpha(char c);
  bool is_alpha_numeric(char c);
  bool is_digit(char c);
};
