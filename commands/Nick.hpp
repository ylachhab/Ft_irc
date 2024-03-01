#ifndef NICK_HPP
#define NICK_HPP

#include "../InfoClient.hpp"
#include <iostream>
#include <vector>

class Nick : public InfoClient
{
public:
	Nick();
	~Nick();

	bool specialCharacter(std::string &str);
	static void executeNick(std::vector<std::string> &vec);
};



#endif