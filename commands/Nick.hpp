#ifndef NICK_HPP
#define NICK_HPP

#include "Client.hpp"
#include <iostream>
#include <vector>

class Nick : public Client
{
private:
	std::string	_nickName;
public:
	Nick();
	~Nick();

	bool specialCharacter(std::string &str);
	void executeCmd(std::vector<std::string> &vec);
	const std::string& getNickName() const;
};



#endif