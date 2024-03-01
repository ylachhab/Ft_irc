#include "Pass.hpp"

Pass::Pass(){}

Pass::~Pass(){}

void Pass::executePass(std::vector<std::string> &vec)
{
	Pass obj;
	std::cout << obj.password <<"\n";
	if (obj._registred)
		std::cout << "PASS :You may not reregister\n";
	else
	{
		if (vec.size() && !vec[0].empty())
		{
			obj._pass = true;
			if (vec[0].compare(obj.password) == 0)
				obj._authenticated = true;
			else
				obj._authenticated = false;
		}
		else
			std::cerr << "PASS :Not enough parameters\n";
	}
	std::cout << obj._authenticated <<"\n";
	if (obj._authenticated)
		std::cout << "authenticated!!\n";
}
