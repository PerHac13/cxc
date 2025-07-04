#include "compiler.hpp"

Compiler::Compiler(bool show_parse_tree) : show_parse_tree(show_parse_tree) {}

void Compiler::compile(std::string filename)
{
    Parser parser(filename);
    Node *root = parser.parse();

    std::string basename = get_basename(filename);

    if (root == nullptr)
    {
        std::cerr << "Parsing failed for file: " << filename << std::endl;
        return;
    }
    if (show_parse_tree)
    {
        std::cout << "Parse tree for file: " << filename << std::endl;
        Node::print(root);
    }

    Backend backend(basename);
    backend.traverse(root);

    Optimizer optimizer;
    optimizer.optimize(basename + ASM_EXT);

    Node::destroy(root);
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