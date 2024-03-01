#ifndef USER_HPP
#define USER_HPP


#include "Client.hpp"

class User : public Client
{
private:
	std::string	_userName;
	std::string	_hostName;
public:
	User();
	~User();

	void executeCmd(std::vector<std::string> &vec);
	const std::string& getUserName() const;
	const std::string& getHostName() const;
};


#endif