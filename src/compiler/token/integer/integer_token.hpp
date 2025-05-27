#pragma once

#include <string>
#include "../index.hpp"

class IntegerToken : public Token
{
public:
    IntegerToken(std::string value, int line_number);

    static bool is_integer(char character);
};