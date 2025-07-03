#include <fstream>
#include <iostream>
#include <string>

#include "scanner.hpp"
#include "state_transition_table/state_transition_table.hpp"

Scanner::Scanner(std::string filename) : line_number(1), next_char(' '), is_eof_reached(false)
{
    file.open(filename);
    check_file(file, filename);
}

Scanner::~Scanner()
{
    close_file();
}

void Scanner::check_file(std::ifstream &file, const std::string &filename)
{
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file '" << filename << "'." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Scanner::close_file()
{
    if (file.is_open())
    {
        file.close();
    }
}

void Scanner::check_for_eof()
{
    if (file.eof())
    {
        is_eof_reached = true;
        next_char = ' ';
    }
}

void Scanner::check_for_invalid_character(int state)
{
    if (StateTransitionTable::is_invalid_char(next_char) && state != COMMENT_STATE)
    {
        StateTransitionTable::print_invalid_char_error(next_char, line_number);
        exit(1);
    }
}

void Scanner::check_for_table_error(int state)
{
    if (is_table_error(state))
    {
        StateTransitionTable::print_error(state, line_number);
        exit(1);
    }
}

Token Scanner::read()
{
    int state = 0;
    int next_state = StateTransitionTable::get_next_state(state, next_char);
    std::string string = "";

    do
    {
        check_for_eof();

        check_for_invalid_character(state);

        next_state = StateTransitionTable::get_next_state(state, next_char);

        check_for_table_error(next_state);

        if (StateTransitionTable::is_final_state(next_state))
        {
            return StateTransitionTable::get_token(next_state, string, line_number);
        }
        else
        {
            state = next_state;
            if (!isspace(next_char) && next_char != COMMENT_CHAR && state != COMMENT_STATE)
            {
                string += next_char;
            }
        }
        if (next_char == '\n')
        {
            line_number++;
        }
    } while (file >> std::noskipws >> next_char || !is_eof_reached);

    return EOFToken(line_number);
}
