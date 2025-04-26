#include <iostream>
#include "main_functions.hpp"
#include "error_reporter/error_reporter.hpp"
#include "token/token.hpp"
#include "data_structures/hash_table.hpp"
#include <cstdlib>

/*******************************************************************************
    Only the entry point (main) function should be in this file because this
    file is excluded from testing. 
*******************************************************************************/

int main (int argc, char** argv) {

    Error_Reporter e;

    if (argc > 2) {
        std::cout << "Usage: lox [filename]" << std::endl;
        return 64;
    } else if (argc == 2) {
        uint64_t ret_code = runFile(argv[1], e);
        return ret_code;
    } else {
        runPrompt(e);
    }

    return 0;

}
