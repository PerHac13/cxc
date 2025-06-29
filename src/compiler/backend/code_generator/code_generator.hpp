#pragma once

#include <string>
#include <fstream>

#include "../target_keywords.hpp"

const std::string ASM_EXT = ".asm";

class CodeGenerator
{
public:
    CodeGenerator(std::string base_filename);
    ~CodeGenerator();
    void print_to_target(std::string str);
    void print_label(std::string label);

    std::string get_tmp_var();
    std::string get_and_store_tmp_var();
    std::string get_label();

    void set_tmp_vars_to_zero();
    void close_target();

private:
    std::ofstream target;
    int tmp_var_count;
    int label_count;
};