#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <stdio.h>

#include "optimizer.hpp"
#include "../target_keywords.hpp"

void Optimizer::optimize(std::string target_filename)
{
    target.open(target_filename);
    std::ofstream tmp;
    std::string tmp_filename = "tmp_" + target_filename;
    tmp.open(tmp_filename);

    if (!target.is_open() || !tmp.is_open())
    {
        std::cerr << "Error: Could not open file '" << target_filename << "'." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string prev_line;
    std::string line;
    while (std::getline(target, line))
    {
        bool print_to_tmp = true;

        if (is_line(prev_line, STACK_READ) && is_line(line, STACK_WRITE))
        {
            std::vector<std::string> prev_line_words = split_line(prev_line);
            std::vector<std::string> line_words = split_line(line);

            int prev_location = to_int(prev_line_words[1]);
            int location = to_int(line_words[1]);
            if (prev_location == location)
            {
                print_to_tmp = false;
            }
        }
        if (print_to_tmp)
        {
            tmp << line << std::endl;
        }
        prev_line = line;
    }
    std::getline(target, line); // Read the last line

    tmp.close();
    target.close();

    if (remove(target_filename.c_str()) != 0)
    {
        std::cerr << "Error: Could not remove file '" << target_filename << "'." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (rename(tmp_filename.c_str(), target_filename.c_str()) != 0)
    {
        std::cerr << "Error: Could not rename file '" << tmp_filename << "' to '" << target_filename << "'." << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool Optimizer::is_line(std::string line, std::string target_keyword)
{
    return line.find(target_keyword) != std::string::npos;
}

std::vector<std::string> Optimizer::split_line(std::string line)
{
    std::istringstream iss(line);
    return std::vector<std::string>(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>());
}

int Optimizer::to_int(std::string str)
{
    try
    {
        int num;
        std::istringstream iss(str);
        iss >> num;
        if (iss.fail() || !iss.eof())
        {
            throw std::invalid_argument("Invalid integer format");
        }
        return num;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Error: Invalid integer conversion for string '" << str << "'." << std::endl;
        exit(EXIT_FAILURE);
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Error: Integer conversion out of range for string '" << str << "'." << std::endl;
        exit(EXIT_FAILURE);
    }
}