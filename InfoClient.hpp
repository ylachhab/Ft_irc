#ifndef INFOCLIENT
#define INFOCLIENT

#include <iostream>
#include <vector>

class InfoClient
{
protected:
	std::string	_userName;
	std::string	_hostName;
	std::string	_nickName;
	bool		_authenticated;
	bool		_pass;
	bool		_nick;
	bool		_user;
	bool		_registred;
public:
	std::string	password;
	InfoClient();
	InfoClient(const InfoClient& obj);
	InfoClient &operator=(const InfoClient& obj);
	~InfoClient();

	const std::string& getPassword() const;
	// void setPassword(const std::string& pass);
};


#endif