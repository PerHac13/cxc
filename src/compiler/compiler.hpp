#pragma once

#include <string>
#include <iostream>
#include "frontend/frontend.hpp"
#include "backend/backend.hpp"

class Compiler
{
public:
    Compiler(bool show_parse_tree = false);
    void compile(std::string filename);

private:
    bool show_parse_tree = false;
    std::string get_basename(std::string filename);
};