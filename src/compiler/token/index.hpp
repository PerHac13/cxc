#pragma once

#include <string>
#include <fstream>
#include "token_type_id/token_type_id.hpp"

class Token
{
public:
    Token();
    Token(TokenTypeId type_id, const char value);
    Token(TokenTypeId type_id, const std::string &value);
    Token(TokenTypeId type_id, const std::string &value, int line_number);

    bool is_eof() const;
    bool is_identifier() const;
    bool is_integer() const;
    std::string get_value();

private:
    TokenTypeId type_id;
    std::string value;
    int line_number;

    friend std::ostream &operator<<(std::ostream &os, const Token &token);
    friend bool operator==(const Token &lhs, const Token &rhs);
    friend bool operator!=(const Token &lhs, const Token &rhs);
    friend bool operator<(const Token &lhs, const Token &rhs);
};