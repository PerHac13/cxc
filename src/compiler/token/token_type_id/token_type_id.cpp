#include "tocken_type_id.hpp"

bool is_token_type_id(int num)
{

    switch (num)
    {
    case DefaultTokenId:
    case DelimiterTokenId:
    case EOFTokenId:
    case IdentifierTokenId:
    case IntegerTokenId:
    case KeywordTokenId:
    case OperatorTokenId:
        return true;
    default:
        return false;
    }
}

std::string get_token_type_id_name(int num)
{
    switch (num)
    {
    case DelimiterTokenId:
        return "DelimiterTokenId";
    case EOFTokenId:
        return "EOFTokenId";
    case IdentifierTokenId:
        return "IdentifierTokenId";
    case IntegerTokenId:
        return "IntegerTokenId";
    case KeywordTokenId:
        return "KeywordTokenId";
    case OperatorTokenId:
        return "OperatorTokenId";
    case DefaultTokenId:
        return "DefaultTokenId";

    default:
        return "DefaultTokenId";
    }
}