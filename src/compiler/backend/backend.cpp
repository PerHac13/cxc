#include <iostream>

#include "backend.hpp"

const std::set<std::string> Backend::new_scope_labels = {
    BLOCK, IFSTAT, LOOP};

const std::set<std::string> Backend::labels_containing_expr = {
    ASSIGN, OUT, IFSTAT, LOOP};

Backend::Backend(std::string base_filename)
    : code_generator(new CodeGenerator(base_filename)) {}

void Backend::traverse(Node *node)
{
    if (!node)
    {
        return;
    }

    if (introduces_new_scope(node))
    {
        var_stack.push();
        code_generator->print_to_target(PUSH);
    }

    check_for_variable(node);

    check_for_print_statements(node);

    check_for_negation(node);

    check_for_r_letter(node);

    check_for_expr(node);

    check_for_expr(node);

    check_for_ifstat(node);

    check_for_loop(node);

    if (does_not_contain_expression(node))
    {
        traverse_children(node);
    }

    if (introduces_new_scope(node))
    {
        // Exiting a scope
        pop_vars_in_current_scope();
        var_stack.pop();
        code_generator->print_to_target(POP);
    }

    if (node->label == START)
    {
        pop_vars_in_current_scope();
        code_generator->print_to_target(STOP);
        code_generator->set_tmp_vars_to_zero();
        code_generator->close_target();
    }
}

void Backend::pop_vars_in_current_scope()
{
    int num_vars_in_curr_scope = var_stack.num_vars_in_current_scope();
    for (int i = 0; i < num_vars_in_curr_scope; ++i)
    {
        code_generator->print_to_target(POP);
    }
}

void Backend::print_error_and_exit(std::string err_msg, Token token)
{
    std::cerr << "Error: " << err_msg << " with token " << token
              << std::endl;
    exit(EXIT_FAILURE);
}

bool Backend::introduces_new_scope(Node *node)
{
    return new_scope_labels.find(node->label) != new_scope_labels.end();
}

void Backend::traverse_children(Node *node)
{
    for (size_t i = 0; i < node->children.size(); ++i)
    {
        // traverse_child(node, i);
        traverse(node->children[i]);
    }
}

void Backend::traverse_child(Node *node, int child_idx)
{

    if (child_idx < 0 || child_idx >= static_cast<int>(node->children.size()))
    {
        print_error_and_exit("Child index out of bounds", node->tokens[0]);
    }

    Node *child = node->children[child_idx];
    if (child)
    {
        traverse_children(child);
    }
}

bool Backend::contains_variable_declarations(Node *node)
{
    return node->label == VARS;
}

bool Backend::does_not_contain_expression(Node *node)
{
    return !contains_expression(node);
}

bool Backend::contains_expression(Node *node)
{
    return labels_containing_expr.find(node->label) != labels_containing_expr.end() ||
           (node->label == EXPR && node->children.size() == 2) ||
           (node->label == HASH && node->tokens.size() == 1);
}

void Backend::check_location(int location, std::string err_msg, Token token)
{
    if (location < 0)
    {
        print_error_and_exit(err_msg, token);
    }
}

void Backend::check_for_variable(Node *node)
{
    std::vector<Token> identifiers = node->get_identifiers_tokens();
    for (const Token &token : identifiers)
    {
        if (contains_variable_declarations(node))
        {
            int location = var_stack.find(token);
            if (location == 0)
            {
                print_error_and_exit("Duplicate variable declaration", token);
            }
            code_generator->print_to_target(PUSH);
            var_stack.add(token);
        }
        else
        {
            int location = var_stack.find(token);
            if (location == -1)
            {
                print_error_and_exit("Variable not declared", token);
            }
            check_for_assignments(node, location);
            check_for_input_statements(node, location);
        }
    }
}

void Backend::check_for_assignments(Node *node, int location)
{
    if (node->label == ASSIGN)
    {
        traverse_children(node);
        code_generator->print_to_target(STACK_WRITE + " " + std::to_string(location));
    }
}

void Backend::check_for_input_statements(Node *node, int location)
{
    if (node->label == IN)
    {
        std::string tmp_var = code_generator->get_tmp_var();

        code_generator->print_to_target(READ + " " + tmp_var);

        code_generator->print_to_target(LOAD + " " + tmp_var);

        code_generator->print_to_target(STACK_WRITE + " " + std::to_string(location));
    }
}

void Backend::check_for_negation(Node *node)
{
    if (node->label == HASH && node->tokens.size() == 1)
    {
        traverse_children(node);
        code_generator->print_to_target(MULT + " -1");
    }
}

void Backend::check_for_r_letter(Node *node)
{
    if (node->label == R_LETTER)
    {
        for (auto token : node->tokens)
        {

            if (token.is_identifier())
            {
                int location = var_stack.find(token);

                code_generator->print_to_target(STACK_READ + " " + std::to_string(location));
            }

            if (token.is_integer())
            {
                code_generator->print_to_target(LOAD + " " + token.get_value());
            }
            else
            {
                print_error_and_exit("Expected identifier or integer", token);
            }
        }
    }
}

void Backend::check_for_expr(Node *node)
{
    if (node->label == EXPR && node->children.size() == 2)
    {
        traverse_child(node, 1);
        std::string tmp_var = code_generator->get_and_store_tmp_var();
        traverse_child(node, 0);

        code_generator->print_to_target(get_operation(node->tokens[0]) + " " + tmp_var);
    }
}

void Backend::check_for_print_statements(Node *node)
{
    if (node->label == OUT)
    {
        traverse_children(node);
        std::string tmp_var = code_generator->get_and_store_tmp_var();

        code_generator->print_to_target(WRITE + " " + tmp_var);
    }
}

void Backend::check_for_ifstat(Node *node)
{
    if (node->label == IFSTAT)
    {
        std::string out_label = code_generator->get_label();
        evaluate_condition(node, out_label);
        code_generator->print_label(out_label);
    }
}

void Backend::check_for_loop(Node *node)
{
    if (node->label == LOOP)
    {
        std::string loop_label = code_generator->get_label();
        std::string out_label = code_generator->get_label();

        code_generator->print_label(loop_label);

        evaluate_condition(node, out_label);

        code_generator->print_to_target(BREAK + " " + loop_label);
        code_generator->print_label(out_label);
    }
}

std::string Backend::get_operation(Token operation_token)
{
    if (OperatorToken::is_addition_token(operation_token))
    {
        return ADD;
    }
    else if (OperatorToken::is_subtraction_token(operation_token))
    {
        return SUB;
    }
    else if (OperatorToken::is_multiplication_token(operation_token))
    {
        return MULT;
    }
    else if (OperatorToken::is_division_token(operation_token))
    {
        return DIV;
    }
    else
    {
        print_error_and_exit("Invalid operation token", operation_token);
        return ""; // Unreachable, but avoids compiler warning
    }
}

std::vector<std::string> Backend::get_break_conditions(Token relational_token)
{
    std::vector<std::string> break_conditions;
    if (OperatorToken::is_less_than_token(relational_token))
    {
        break_conditions.push_back(BREAK_ZERO_OR_POSITIVE);
    }
    else if (OperatorToken::is_greater_than_token(relational_token))
    {
        break_conditions.push_back(BREAK_ZERO_OR_NEGATIVE);
    }
    else if (OperatorToken::is_equals_token(relational_token))
    {
        break_conditions.push_back(BREAK_NEGATIVE);
        break_conditions.push_back(BREAK_POSITIVE);
    }

    return break_conditions;
}

void Backend::evaluate_condition(Node *node, std::string out_label)
{
    traverse_child(node, 2);

    std::string temp_var = code_generator->get_and_store_tmp_var();

    traverse_child(node, 0);

    code_generator->print_to_target(SUB + " " + temp_var);

    Node *second_child = node->children[1];
    Token relational_operator = second_child->tokens[0];

    std::vector<std::string> break_conditions = get_break_conditions(relational_operator);
    for (auto break_condition : break_conditions)
    {
        code_generator->print_to_target(break_condition + " " + out_label);
    }

    traverse_child(node, 3);
}
