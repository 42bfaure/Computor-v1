#include "../headers/Computor.hpp"

int main(int argc, char **argv)
{
    try {
        if (argc != 2)
        {
            std::cerr << "Usage: " << argv[0] << " <equation>" << std::endl;
            return 1;
        }
        std::cout << "Equation: " << argv[1] << std::endl;
        Computor computor(argv[1]);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}