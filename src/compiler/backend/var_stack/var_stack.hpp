#pragma once

#include <iostream>
#include <deque>
#include <set>
#include <string>

#include "../../token/token.hpp"

class VarStack
{
public:
    VarStack();
    void push();
    void pop();
    void add(Token var);
    void remove(Token var);
    int find(Token var);
    int num_vars_in_current_scope();

private:
    std::deque<std::set<Token>> var_stack;
};