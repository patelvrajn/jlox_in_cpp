#include "error_reporter/error_reporter.hpp"

#include <iostream>

void Error_Reporter::error(const uint64_t line, const std::string& message) {
  std::cerr << "[line " << line << "] Error: " << message << std::endl;
  this->had_error = true;
}
