#pragma once

#include <string>

#include "frontend/frontend.hpp"

class Compiler
{
public:
    void compile(std::string filename);

private:
    std::string get_basename(std::string filename);
};