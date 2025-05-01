#pragma once

#include <string>

#include "error_reporter/error_reporter.hpp"

uint64_t runFile(const std::string& file_path, Error_Reporter& e);
void runPrompt(Error_Reporter& e);
void run(const std::string& source);
