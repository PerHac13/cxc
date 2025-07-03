#include "parser.hpp"

Parser::Parser(std::string filename)
{
    scanner = new Scanner(filename);
    current_token = scanner->read();
}

Parser::~Parser()
{
    delete scanner;
}

Node *Parser::parse()
{
    Node *root = S();
    current_token = scanner->read();
    if (current_token.is_eof())
    {
        return root;
    }
    print_error_and_exit();

    return nullptr;
}

/**
 * <S> -> program <vars> <block>
 */

Node *Parser::S()
{
    int level = 1;
    if (KeywordToken::is_program_token(current_token))
    {
        Node *node = Node::of(START, level);
        current_token = scanner->read();
        node->append_child(vars(level));
        node->append_child(block(level));
        return node;
    }
    print_error_and_exit();
    return nullptr;
}

/**
 * <block> -> start <vars> <stats> end
 */

Node *Parser::block(int level)
{
    ++level;
    if (KeywordToken::is_start_token(current_token))
    {
        Node *node = Node::of(BLOCK, level);
        current_token = scanner->read();
        node->append_child(vars(level));
        node->append_child(stats(level));
        if (KeywordToken::is_end_token(current_token))
        {
            current_token = scanner->read();
            return node;
        }
    }
    print_error_and_exit();
    return nullptr;
}

/**
 * <vars> -> var Identifier <vars> | ε
 */

Node *Parser::vars(int level)
{
    if (!KeywordToken::is_var_token(current_token))
    {
        return nullptr; // ε production
    }
    ++level;
    Node *node = Node::of(VARS, level);
    current_token = scanner->read();
    if (current_token.is_identifier())
    {
        node->append_token(current_token);
        current_token = scanner->read();
        node->append_child(vars(level));
        return node;
    }
    print_error_and_exit();
    return nullptr;
}

/**
 * <expr> -> <H> + <expr> | <H> - <expr>
 * <expr> -> <H> / <expr> | <H> * <expr>
 * <expr> -> <H>
 */
Node *Parser::expr(int level)
{
    level++;
    Node *node = Node::of(EXPR, level);
    node->append_child(H(level));
    if (OperatorToken::is_addition_token(current_token))
    {
        node->append_token(current_token);
        current_token = scanner->read();
        node->append_child(expr(level));
    }
    else if (OperatorToken::is_subtraction_token(current_token))
    {
        node->append_token(current_token);
        current_token = scanner->read();
        node->append_child(expr(level));
    }
    else if (OperatorToken::is_division_token(current_token))
    {
        node->append_token(current_token);
        current_token = scanner->read();
        node->append_child(expr(level));
    }
    else if (OperatorToken::is_multiplication_token(current_token))
    {
        node->append_token(current_token);
        current_token = scanner->read();
        node->append_child(expr(level));
    }

    return node;
}

/**
 * <H> -> # <R> | <R>
 */

Node *Parser::H(int level)
{
    ++level;
    Node *node = Node::of(HASH, level);
    if (OperatorToken::is_negation_token(current_token))
    {
        node->append_token(current_token);
        current_token = scanner->read();
        node->append_child(R(level));
    }
    else
    {
        node->append_child(R(level));
    }

    return node;
}

/**
 * <R> ->  ( <expr> ) | Identifier | Integer
 */

Node *Parser::R(int level)
{
    ++level;
    Node *node = Node::of(R_LETTER, level);

    if (DelimiterToken::is_left_parentheses_token(current_token))
    {
        current_token = scanner->read();
        node->append_child(expr(level));
        if (DelimiterToken::is_right_parentheses_token(current_token))
        {
            current_token = scanner->read();
            return node;
        }
    }
    else if (current_token.is_identifier())
    {
        node->append_token(current_token);
        current_token = scanner->read();
        return node;
    }
    else if (current_token.is_integer())
    {
        node->append_token(current_token);
        current_token = scanner->read();
        return node;
    }
    print_error_and_exit();
    return nullptr;
}

/**
 * <stats> -> <stat> <m_stat>
 */
Node *Parser::stats(int level)
{
    ++level;
    Node *node = Node::of(STAT, level);
    node->append_child(stat(level));
    node->append_child(m_stat(level));
    return node;
}

/**
 * <m_stat> -> <stats> | ε
 */
Node *Parser::m_stat(int level)
{
    if (!is_first_of_stats(current_token))
        return nullptr; // ε production

    ++level;
    Node *node = Node::of(M_STAT, level);
    node->append_child(stats(level));
    return node;
}

bool Parser::is_first_of_stats(Token token)
{
    return KeywordToken::is_read_token(token) ||
           KeywordToken::is_print_token(token) ||
           KeywordToken::is_start_token(token) ||
           KeywordToken::is_if_token(token) ||
           KeywordToken::is_iter_token(token) ||
           KeywordToken::is_let_token(token);
}

/**
 * <stat> -> <in> , | <out> , | <block> , | <ifstat> , | <loop> , | <assign> ,
 */
Node *Parser::stat(int level)
{
    level++;
    Node *node = Node::of(STAT, level);
    if (KeywordToken::is_read_token(current_token))
    {
        node->append_child(in(level));
        check_for_comma_token();
        return node;
    }
    else if (KeywordToken::is_print_token(current_token))
    {
        node->append_child(out(level));
        check_for_comma_token();
        return node;
    }
    else if (KeywordToken::is_start_token(current_token))
    {
        node->append_child(block(level));
        check_for_comma_token();
        return node;
    }
    else if (KeywordToken::is_if_token(current_token))
    {
        node->append_child(ifstat(level));
        check_for_comma_token();
        return node;
    }
    else if (KeywordToken::is_iter_token(current_token))
    {
        node->append_child(loop(level));
        check_for_comma_token();
        return node;
    }
    else if (KeywordToken::is_let_token(current_token))
    {
        node->append_child(assign(level));
        check_for_comma_token();
        return node;
    }
    print_error_and_exit();
    return nullptr;
}

void Parser::check_for_comma_token()
{
    if (DelimiterToken::is_comma_token(current_token))
    {
        current_token = scanner->read();
    }
    else
    {
        print_error_and_exit();
    }
}

/**
 * <in> -> read Identifier
 */
Node *Parser::in(int level)
{
    if (KeywordToken::is_read_token(current_token))
    {
        level++;
        Node *node = Node::of(IN, level);
        current_token = scanner->read();
        if (current_token.is_identifier())
        {
            node->append_token(current_token);
            current_token = scanner->read();
            return node;
        }
    }
    print_error_and_exit();
    return nullptr;
}

/**
 * <out> -> print <expr>
 */
Node *Parser::out(int level)
{
    level++;
    if (KeywordToken::is_print_token(current_token))
    {
        Node *node = Node::of(OUT, level);
        current_token = scanner->read();
        node->append_child(expr(level));
        return node;
    }
    print_error_and_exit();
    return nullptr;
}

/**
 * <ifstat> -> if ( <expr> <O> <expr> ) <stat>
 */
Node *Parser::ifstat(int level)
{
    level++;
    if (KeywordToken::is_if_token(current_token))
    {
        Node *node = Node::of(IFSTAT, level);
        current_token = scanner->read();
        if (DelimiterToken::is_left_parentheses_token(current_token))
        {
            current_token = scanner->read();
            node->append_child(expr(level));
            node->append_child(O(level));
            node->append_child(expr(level));
            if (DelimiterToken::is_right_parentheses_token(current_token))
            {
                current_token = scanner->read();
                node->append_child(stat(level));
                return node;
            }
        }
    }
    print_error_and_exit();
    return nullptr;
}

/**
 * <loop> -> iter ( <expr> <O> <expr> ) <stat>
 */
Node *Parser::loop(int level)
{
    level++;
    if (KeywordToken::is_iter_token(current_token))
    {
        Node *node = Node::of(LOOP, level);
        current_token = scanner->read();
        if (DelimiterToken::is_left_parentheses_token(current_token))
        {
            current_token = scanner->read();
            node->append_child(expr(level));
            node->append_child(O(level));
            node->append_child(expr(level));
            if (DelimiterToken::is_right_parentheses_token(current_token))
            {
                current_token = scanner->read();
                node->append_child(stat(level));
                return node;
            }
        }
    }
    print_error_and_exit();
    return nullptr;
}

/**
 * <assign> -> let Identifier = <expr>
 */
Node *Parser::assign(int level)
{
    level++;
    if (KeywordToken::is_let_token(current_token))
    {
        Node *node = Node::of(ASSIGN, level);
        current_token = scanner->read();
        if (current_token.is_identifier())
        {
            node->append_token(current_token);
            current_token = scanner->read();
            if (OperatorToken::is_assignment_token(current_token))
            {
                current_token = scanner->read();
                node->append_child(expr(level));
                return node;
            }
        }
    }
    print_error_and_exit();
    return nullptr;
}

/**
 * <O> -> < | > | :
 */
Node *Parser::O(int level)
{
    level++;
    Node *node = Node::of(OPERATOR, level);
    if (is_O_token(current_token))
    {
        node->append_token(current_token);
        current_token = scanner->read();
        return node;
    }
    print_error_and_exit();
    return nullptr;
}

bool Parser::is_O_token(Token token)
{
    return OperatorToken::is_less_than_token(token) ||
           OperatorToken::is_greater_than_token(token) ||
           OperatorToken::is_equals_token(token);
}

void Parser::print_error_and_exit()
{
    std::cerr << "Parser print_error_and_exit: " << current_token << std::endl;
    exit(1);
}