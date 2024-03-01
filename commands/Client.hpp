#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class Client
{
protected:
	std::string _rellPass;
	bool		_authenticated;
	bool		_pass;
	bool		_nick;
	bool		_user;
	bool		_registred;

public:
	Client();
	virtual ~Client();

	virtual void executeCmd(std::vector<std::string> &vec) = 0;
};

#endif