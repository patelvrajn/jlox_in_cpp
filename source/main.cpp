#include <cstdlib>
#include <iostream>

#include "data_structures/hash_table.hpp"
#include "error_reporter/error_reporter.hpp"
#include "main_functions.hpp"
#include "token/token.hpp"

/*******************************************************************************
    Only the entry point (main) function should be in this file because this
    file is excluded from testing.
*******************************************************************************/

int main(int argc, char** argv) {
  Error_Reporter e;

  if (argc > 2) {
    std::cout << "Usage: jlox_in_cpp [filename]" << std::endl;
    return 64;
  } else if (argc == 2) {
    uint64_t ret_code = run_file(argv[1], e);
    return ret_code;
  } else {
    run_prompt(e);
  }

  return 0;
}
