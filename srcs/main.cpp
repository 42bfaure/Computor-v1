#include "../headers/Computor.hpp"

/**
 * @file
 * @brief Program entry point for polynomial equation solving.
 */

/**
 * @brief Run the CLI interface for `Computor`.
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument values.
 * @return `0` on success, `1` when arguments are invalid.
 */
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <equation>" << std::endl;
        return 1;
    }
    std::cout << "Equation: " << argv[1] << std::endl;
    Computor computor(argv[1]);
    return 0;
}