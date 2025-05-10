#include "main_functions.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "data_structures/dynamic_array.hpp"
#include "scanner/scanner.hpp"
#include "token/token.hpp"

uint64_t run_file(const std::string& file_path, Error_Reporter& e) {
  // Get file contents as a string.
  std::ifstream file(file_path);
  std::ostringstream oss;
  oss << file.rdbuf();
  file.close();

  // Execute the code in the file.
  run(oss.str(), e);

  if (e.had_error) {
    return 65;
  }

  return 0;
}

void run_prompt(Error_Reporter& e) {
  while (true) {
    std::string line;

    std::cout << "> ";
    std::getline(std::cin, line);

    if (line.empty()) {
      break;
    }

    run(line, e);

    e.had_error = false;
  }
}

void run(const std::string& source, Error_Reporter& e) {
  Scanner s(source, e);
  Dynamic_Array<Token> tokens = s.scan_tokens();

  for (Token token : tokens) {
    std::cout << token.to_string() << std::endl;
  }
}
