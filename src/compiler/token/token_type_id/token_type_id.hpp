#pragma once

#include <string>

typedef enum
{
    DefaultTokenId = 1000,

    DelimiterTokenId = 10000,
    EOFTokenId = 10001,
    IdentifierTokenId = 10002,
    IntegerTokenId = 10003,
    KeywordTokenId = 10004,
    OperatorTokenId = 10005,

} TokenTypeId;

bool is_token_type_id(int num);
std::string get_token_type_id_name(int num);