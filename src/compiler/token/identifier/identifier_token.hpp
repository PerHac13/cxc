#pragma once

#include "../index.hpp"

class IdentifierToken : public Token
{
public:
    IdentifierToken(std::string value, int line_number);

    static bool is_identifier(Token token);
};