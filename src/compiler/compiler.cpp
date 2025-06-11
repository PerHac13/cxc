#include "compiler.hpp"

void Compiler::compile(std::string filename)
{
    Parser parser(filename);
    Node *root = parser.parse();

    std::string basename = get_basename(filename);

    if (root != nullptr)
    {
        Node::print(root);
        Node::destroy(root);
    }
    else
    {
        std::cerr << "Parsing failed for file: " << basename << std::endl;
    }
}

std::string Compiler::get_basename(std::string filename)
{

    size_t last_index = filename.find_last_of('.');
    if (last_index == std::string::npos)
    {
        return filename;
    }
    return filename.substr(0, last_index);
}