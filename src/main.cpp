#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

#include "timer/timer.hpp"
#include "compiler/compiler.hpp"

std::string get_filename(int argc, char *argv[]);

int main(int argc, char *argv[])
{

    Timer timer = Timer();
    timer.start("Compiler program started");

    std::string filename = get_filename(argc, argv);

    // Compiler entry point
    Compiler compiler;
    compiler.compile(filename);
    // Compiler end point

    std::cout << "Success :)" << std::endl;

    timer.stop("Compiler program finished");

    std::cout << "Total elapsed time: " << timer.time_val() << " seconds." << std::endl;
    return 0;
}

std::string get_filename(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error: No input file provided. \n"
                  << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        std::cerr << "Please provide a valid input file." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (argc != 2)
    {
        std::cerr << "Error: Too many arguments provided. \n"
                  << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string(argv[1]);
}