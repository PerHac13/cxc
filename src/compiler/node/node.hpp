#pragma once

#include <string>
#include <vector>

#include "node_labels.hpp"
#include "../token/token.hpp"

class Node
{
public:
    static Node *of(std::string label, int level);
    static void destroy(Node *node);
    void append_child(Node *child);
    void append_token(Token token);
    static void print(Node *node);

    std::vector<Token> get_identifiers_tokens();
    std::vector<Node *> children;
    std::vector<Token> tokens;
    std::string label;
    int level;

private:
    static void print_node(Node *node);
    static void print_children(Node *node);

    friend std::ostream &operator<<(std::ostream &, const Node &);
};