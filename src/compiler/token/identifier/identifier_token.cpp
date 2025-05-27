#include "identifier_token.hpp"

IdentifierToken::IdentifierToken(std::string value, int line_number)
    : Token(IdentifierTokenId, std::move(value), line_number) {}