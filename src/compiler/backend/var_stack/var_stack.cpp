#include "./var_stack.hpp"

VarStack::VarStack()
{
    var_stack.emplace_back();
}

void VarStack::push()
{
    var_stack.emplace_back();
}

void VarStack::pop()
{
    if (!var_stack.empty())
    {
        var_stack.pop_back();
    }
}

void VarStack::add(Token var)
{
    if (!var_stack.empty())
    {
        var_stack.back().insert(var);
    }
}

void VarStack::remove(Token var)
{
    if (!var_stack.empty())
    {
        var_stack.back().erase(var);
    }
}

/**
 * Finds a variable and returns the distance from the top of the stack.
 *
 * Returns -1 if not found.
 */

int VarStack::find(Token var)
{
    int idx_stack_top = 0;
    for (int i = var_stack.size() - 1; i >= 0; i--)
    {
        std::set<Token> tokens = var_stack[i];
        for (auto token = tokens.rbegin(); token != tokens.rend(); ++token)
        {
            if (var == *token)
            {
                return idx_stack_top;
            }
            ++idx_stack_top;
        }
        ++idx_stack_top;
    }
    return -1;
}

int VarStack::num_vars_in_current_scope()
{
    if (!var_stack.empty())
    {
        return var_stack.back().size();
    }
    return 0;
}