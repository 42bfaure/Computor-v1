#include "../headers/Computor.hpp"
#include "../headers/utils.hpp"
#include <cstring>

int main(int argc, char **argv)
{
	try {
		if (argc < 2)
		{
			Computor computor;
			std::string line;
			std::cerr << "Usage: " << argv[0] << " <equation>" << std::endl;
			if (std::getline(std::cin, line) && !line.empty())
			{
					computor.setEquation(line);
					computor.Calcul();
					computor.display();
					return 0;
			}
			else
			{
				std::cerr << "No equation provided" << std::endl;
				return 1;
			}
		}
		Computor computor(argv[1]);
		if (argc > 2 && std::strcmp(argv[2], "--verbose") == 0)
			computor.setVerbose(true);
		computor.Calcul();
		computor.display();
		return 0;
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}