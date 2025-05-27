#include "delimiter_token.hpp"

const char DOT = '.';
const char COMMA = ',';
const char SEMICOLON = ';';
const char LEFT_PARENTHESES = '(';
const char RIGHT_PARENTHESES = ')';
const char LEFT_CURLY_BRACE = '{';
const char RIGHT_CURLY_BRACE = '}';
const char LEFT_SQUARE_BRACKET = '[';
const char RIGHT_SQUARE_BRACKET = ']';

std::set<char> DelimiterToken::delimiters = {
    DOT,
    COMMA,
    SEMICOLON,
    LEFT_PARENTHESES,
    RIGHT_PARENTHESES,
    LEFT_CURLY_BRACE,
    RIGHT_CURLY_BRACE,
    LEFT_SQUARE_BRACKET,
    RIGHT_SQUARE_BRACKET};

DelimiterToken::DelimiterToken(char value)
    : Token(DelimiterTokenId, value) {}

DelimiterToken::DelimiterToken(std::string value, int line_number)
    : Token(DelimiterTokenId, value, line_number) {}

bool DelimiterToken::is_delimiter(char character)
{
    return delimiters.find(character) != delimiters.end();
}

bool DelimiterToken::is_dot_token(Token token)
{
    return token == DelimiterToken(DOT);
}

bool DelimiterToken::is_comma_token(Token token)
{
    return token == DelimiterToken(COMMA);
}

bool DelimiterToken::is_semi_colon_token(Token token)
{
    return token == DelimiterToken(SEMICOLON);
}

bool DelimiterToken::is_left_parentheses_token(Token token)
{
    return token == DelimiterToken(LEFT_PARENTHESES);
}

bool DelimiterToken::is_right_parentheses_token(Token token)
{
    return token == DelimiterToken(RIGHT_PARENTHESES);
}

bool DelimiterToken::is_left_curly_brace_token(Token token)
{
    return token == DelimiterToken(LEFT_CURLY_BRACE);
}

bool DelimiterToken::is_right_curly_brace_token(Token token)
{
    return token == DelimiterToken(RIGHT_CURLY_BRACE);
}

bool DelimiterToken::is_left_square_bracket_token(Token token)
{
    return token == DelimiterToken(LEFT_SQUARE_BRACKET);
}

bool DelimiterToken::is_right_square_bracket_token(Token token)
{
    return token == DelimiterToken(RIGHT_SQUARE_BRACKET);
}
