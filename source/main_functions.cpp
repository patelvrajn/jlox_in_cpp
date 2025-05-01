#include "main_functions.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "data_structures/dynamic_array.hpp"
#include "data_structures/hash_table.hpp"

uint64_t runFile(const std::string& file_path, Error_Reporter& e) {
  std::ifstream file(file_path);
  std::ostringstream oss;
  oss << file.rdbuf();
  file.close();
  run(oss.str());

  if (e.had_error) {
    return 65;
  }

  return 0;
}

void runPrompt(Error_Reporter& e) {
  while (true) {
    std::string line;

    std::cout << "> " << std::endl;
    std::getline(std::cin, line);

    if (line.empty()) {
      break;
    }

    run(line);

    e.had_error = false;
  }
}

void run(const std::string& source) {
  // TODO.
}
