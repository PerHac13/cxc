#include "node.hpp"
#include <algorithm>
#include <iostream>

Node *Node::of(std::string label, int level)
{
    Node *node = new Node();
    node->label = std::move(label);
    node->level = level;
    return node;
}

void Node::destroy(Node *node)
{
    if (node)
    {
        for (auto child : node->children)
        {
            destroy(child);
        }
        delete node;
    }
}

void Node::append_child(Node *child)
{
    if (child)
    {
        children.push_back(child);
    }
}

void Node::append_token(Token token)
{
    tokens.push_back(std::move(token));
}

void Node::print(Node *node)
{
    if (node)
    {
        print_node(node);
        print_children(node);
    }
}

void Node::print_node(Node *node)
{
    printf("%*c%d:%-9s ", node->level, ' ', node->level, node->label.c_str());
    for (auto i = node->tokens.begin(); i != node->tokens.end(); ++i)
        std::cout << *i << ' ';
    printf("\n");
}

void Node::print_children(Node *node)
{
    std::vector<Node *> children = node->children;
    for (unsigned int i = 0; i < children.size(); i++)
    {
        print(children.at(i));
    }
}

std::vector<Token> Node::get_identifiers_tokens()
{
    std::vector<Token> identifiers;
    for (const auto &token : tokens)
    {
        if (token.is_identifier())
        {
            identifiers.push_back(token);
        }
    }
    return identifiers;
}

std::ostream &operator<<(std::ostream &stream, const Node &node)
{
    return stream << node.label;
}