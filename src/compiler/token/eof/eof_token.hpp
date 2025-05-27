#pragma once

#include <string>
#include "../index.hpp"

class EOFToken : public Token
{
public:
    static bool is_eof(char character);
    EOFToken(int line_number);
};