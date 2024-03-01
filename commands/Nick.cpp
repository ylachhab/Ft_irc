#include "Nick.hpp"

Nick::Nick(){}

Nick::~Nick(){}

bool Nick::specialCharacter(std::string &str)
{
	std::string sp = "-[]\\'^{}";
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (!std::isalpha(*it) || !std::isdigit(*it) || sp.find(*it) == std::string::npos)
			return true;
	}
	return false;
}

void Nick::executeNick(std::vector<std::string> &vec)
{
	Nick obj;
	if (obj._pass)
	{
		puts("hi");
		if (vec.size() && !vec[0].empty())
		{
			if (!obj.specialCharacter(vec[0]))
			{
				obj._nick = true;
				obj._nickName = vec[0];
			}
			else
				std::cerr << "NICK :invalid nickname is given\n";
		}
		else
			std::cerr << "NICK :No nickname given\n";
	}
}