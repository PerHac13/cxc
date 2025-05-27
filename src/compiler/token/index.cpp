#include "index.hpp"

Token::Token() : type_id(DefaultTokenId), value(""), line_number(0) {}

Token::Token(TokenTypeId type_id, const char value)
    : type_id(type_id), value(1, value), line_number(0) {}

Token::Token(TokenTypeId type_id, const std::string &value)
    : type_id(type_id), value(value), line_number(0) {}

Token::Token(TokenTypeId type_id, const std::string &value, int line_number)
    : type_id(type_id), value(value), line_number(line_number) {}

bool Token::is_eof() const
{
    return type_id == EOFTokenId;
}

bool Token::is_identifier() const
{
    return type_id == IdentifierTokenId;
}

bool Token::is_integer() const
{
    return type_id == IntegerTokenId;
}

std::string Token::get_value()
{
    return value;
}

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    std::string type_name = get_token_type_id_name(token.type_id);
    os << "Token(type=" << type_name << ", value='" << token.value
       << "', line_number=" << token.line_number << ")";
    return os;
}

bool operator==(const Token &lhs, const Token &rhs)
{
    return lhs.type_id == rhs.type_id && lhs.value == rhs.value;
}

bool operator!=(const Token &lhs, const Token &rhs)
{
    return !(lhs == rhs);
}

bool operator<(const Token &lhs, const Token &rhs)
{
    return lhs.value < rhs.value;
}