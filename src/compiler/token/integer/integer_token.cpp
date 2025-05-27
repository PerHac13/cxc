#include "integer_token.hpp"

IntegerToken::IntegerToken(std::string value, int line_number)
    : Token(IntegerTokenId, value, line_number) {}

bool IntegerToken::is_integer(char character)
{
    return isdigit(character);
}