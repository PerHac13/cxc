#pragma once

#include <iostream>
#include <string>

#include "../scanner/scanner.hpp"
#include "../../node/node.hpp"
#include "../../token/token.hpp"

class Parser
{
public:
    Parser(std::string filename);
    ~Parser();
    Node *parse();

private:
    Scanner *scanner;
    Token current_token;

    Node *S();
    Node *block(int level);
    Node *vars(int level);
    Node *expr(int level);
    Node *H(int level);
    Node *R(int level);
    Node *stats(int level);
    Node *m_stat(int level);
    bool is_first_of_stats(Token token);
    Node *stat(int level);
    Node *in(int level);
    Node *out(int level);
    Node *ifstat(int level);
    Node *loop(int level);
    Node *assign(int level);
    Node *O(int level);
    bool is_O_token(Token token);
    void print_error_and_exit();
    void check_for_comma_token();
};