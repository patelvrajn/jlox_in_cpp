#pragma once

#include <string>

#include "error_reporter/error_reporter.hpp"

uint64_t run_file(const std::string& file_path, Error_Reporter& e);
void run_prompt(Error_Reporter& e);
void run(const std::string& source, Error_Reporter& e);
