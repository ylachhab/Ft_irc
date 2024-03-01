#include "Nick.hpp"

Nick::Nick(){}

Nick::~Nick(){}

const std::string& Nick::getNickName() const
{
	return this->_nickName;
}

bool Nick::specialCharacter(std::string &str)
{
	std::string sp = "-[]/\/'^{}";
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (!std::isalpha(*it) || !std::isdigit(*it) || sp.find(*it) == std::string::npos)
			return true;
	}
	return false;
}

void Nick::executeCmd(std::vector<std::string> &vec)
{
	if (this->_pass)
	{
		if (vec.size() && !vec[0].empty())
		{
			if (!specialCharacter(vec[0]))
			{
				this->_nick = true;
				this->_nickName = vec[0];
			}
			else
				std::cerr << "NICK :invalid nickname is given\n";
		}
		else
			std::cerr << "NICK :No nickname given\n";
	}
}