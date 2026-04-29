#include "../headers/Computor.hpp"

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