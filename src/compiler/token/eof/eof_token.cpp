#include <stdio.h>
#include "eof_token.hpp"

bool EOFToken::is_eof(char character)
{
    return character == EOF;
}

EOFToken::EOFToken(int line_number)
    : Token(EOFTokenId, std::string("EOF"), line_number) {}