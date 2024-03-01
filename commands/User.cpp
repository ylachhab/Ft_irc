#include "User.hpp"

User::User(){}

User::~User(){}

const std::string &User::getHostName() const
{
	return this->_hostName;
}

const std::string &User::getUserName() const
{
	return this->_userName;
}

void User::executeCmd(std::vector<std::string> &vec)
{

}
