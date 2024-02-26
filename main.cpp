#include "FtIrc.hpp"

int main(int ac, char **av) {
	if (ac != 3)
	{
		std::cout << "Error in the arguments" << std::endl;
		return 1;
	}
	std::string port = av[1];
	std::string password = av[2];
	FtIrc obj(port, password);
}