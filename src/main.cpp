#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <tuple>

#include "timer/timer.hpp"
#include "compiler/compiler.hpp"

std::tuple<std::string, bool> get_arguments(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    Timer timer = Timer();
    timer.start("Compiler program started");

    std::tuple<std::string, bool> args = get_arguments(argc, argv);
    std::string filename = std::get<0>(args);
    bool show_parse_tree = std::get<1>(args);

    // compiler start
    Compiler compiler(show_parse_tree);
    compiler.compile(filename);
    // compiler end

    std::cout << "Success :)" << std::endl;
    timer.stop("Compiler program finished");
    std::cout << "Total elapsed time: " << timer.time_val() << " seconds." << std::endl;

    return 0;
}

std::tuple<std::string, bool> get_arguments(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> [--show-pt]" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string filename;
    bool show_parse_tree = false;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--show-pt")
        {
            show_parse_tree = true;
        }
        else
        {
            filename = arg;
        }
    }

    if (filename.empty())
    {
        std::cerr << "Error: No input file provided.\n";
        exit(EXIT_FAILURE);
    }

    return {filename, show_parse_tree};
}
