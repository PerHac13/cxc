#include <stdarg.h>
#include <iostream>

#include "code_generator.hpp"

CodeGenerator::CodeGenerator(std::string base_filename)
    : tmp_var_count(0), label_count(0)
{
    target.open(base_filename + ASM_EXT);
    if (!target.is_open())
    {
        std::cerr << "Error: Could not open file '" << base_filename + ASM_EXT << "'." << std::endl;
        exit(EXIT_FAILURE);
    }
}

CodeGenerator::~CodeGenerator()
{
    if (target.is_open())
    {
        target.close();
    }
}

void CodeGenerator::print_to_target(std::string str)
{
    target << str << std::endl;
}

void CodeGenerator::print_label(std::string label)
{
    target << label << ": " << std::endl;
}

std::string CodeGenerator::get_tmp_var()
{
    return TEMP_VAR_PREFIX + std::to_string(tmp_var_count++);
}

std::string CodeGenerator::get_and_store_tmp_var()
{
    std::string tmp_var = get_tmp_var();
    print_to_target(STORE + " " + tmp_var);
    return tmp_var;
}

void CodeGenerator::set_tmp_vars_to_zero()
{
    for (int i = 0; i < tmp_var_count; ++i)
    {
        print_to_target(TEMP_VAR_PREFIX + std::to_string(i) + " 0\n");
    }
}

std::string CodeGenerator::get_label()
{
    return LABEL_PREFIX + std::to_string(label_count++);
}

void CodeGenerator::close_target()
{
    if (target.is_open())
    {
        target.close();
    }
}