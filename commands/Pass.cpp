#include "Pass.hpp"

Pass::Pass(){}

Pass::~Pass(){}

void Pass::executeCmd(std::vector<std::string> &vec)
{
	if (this->_registred)
		std::cout << "PASS :You may not reregister\n";
	else
	{
		if (vec.size() && !vec[0].empty())
		{
			this->_pass = true;
			if (vec[0].compare(_rellPass) == 0)
				this->_authenticated = true;
			else
				this->_authenticated = false;
		}
		else
			std::cerr << "PASS :Not enough parameters\n";
	}
}
