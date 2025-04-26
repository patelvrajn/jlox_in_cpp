#pragma once

#include <stdint.h>
#include <string>

class Error_Reporter
{
    public:

        bool had_error = false;

        void error (const uint64_t line, const std::string& message);        

    private:

};
