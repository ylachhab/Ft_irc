#include "InfoClient.hpp"

InfoClient::InfoClient()
{
	this->_registred = false;
}

InfoClient::InfoClient(const InfoClient &obj)
{
	*this = obj;
}

InfoClient &InfoClient::operator=(const InfoClient &obj)
{
	if (this != &obj)
	{
		this->_userName = obj._userName;
		this->_hostName = obj._hostName;
		this->_nickName = obj._nickName;
		this->_authenticated = obj._authenticated;
		this->_pass = obj._pass;
		this->_nick = obj._nick;
		this->_user = obj._user;
		this->_registred = obj._registred;
	}
	return *this;
}

InfoClient::~InfoClient()
{
}

const std::string& InfoClient::getPassword() const
{
	return this->password;
}

// void InfoClient::setPassword(const std::string& pass)
// {
// 	this->password = pass;
// }