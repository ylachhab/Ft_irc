
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

/******************* Utils Functions **********************/
void Client::sendRepance(const std::string& msg)
{
	send(this->_fd, msg.c_str(), msg.size(), 0);
} 
bool specialCharacter(std::string &str)
{
	std::string sp = "-[]\\'^{}";
	std::string num = "0123456789";
	if (num.find(str[0]) != std::string::npos)
		return true;
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (!std::isalpha(*it) && !std::isdigit(*it) && sp.find(*it) == std::string::npos)
			return true;
	}
	return false;
}
/******************* PASS Commande **********************/
void Client::executePass(std::vector<std::string> &vec)
{
	if (this->_registred)
		sendRepance(ERR_ALREADYREGISTERED(this->_nickName, Server::_hostname));
	else
	{
		if (vec.size() && !vec[0].empty())
		{
			this->_pass = true;
			if (vec[0].compare(this->_password) == 0)
				this->_authenticated = true;
			else
			{
				this->_authenticated = false;
				sendRepance(ERR_PASSWDMISMATCH(this->_nickName, Server::_hostname));
			}
		}
		else
			sendRepance(ERR_NEEDMOREPARAMS(this->_nickName, Server::_hostname));
	}
}


/******************* NICK Command **********************/
void Client::executeNick(std::vector<std::string> &vec)
{
	if (this->_pass && this->_authenticated)
	{
		for (size_t i = 0; i < Server::cObjs.size(); i++)
		{
			if (Server::cObjs[i]._nickName == vec[0])
			{
				sendRepance(ERR_NICKNAMEINUSE(this->_nickName, Server::_hostname));
				return ;
			}
		}
		if (vec.size() && !vec[0].empty())
		{
			if (specialCharacter(vec[0]) == 0)
			{
				this->_nick = true;
				this->_nickName = vec[0];
			}
			else
				sendRepance(ERR_ERRONEUSNICKNAME(this->_nickName, Server::_hostname));
		}
		else
			sendRepance(ERR_NONICKNAMEGIVEN(this->_nickName, Server::_hostname));
	}
	else
		sendRepance(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}

/******************* USER Command **********************/
void Client::executeUser(std::vector<std::string> &vec)
{
	if (this->_registred)
		sendRepance(ERR_ALREADYREGISTERED(this->_nickName, Server::_hostname));
	if (this->_pass && this->_nick && this->_authenticated)
	{
		if (vec.size() >= 4)
		{
			this->_userName = vec[0];
			this->_realName = vec[3];
			this->_user = true;
			this->_registred = true;
			sendRepance(RPL_WELCOME(this->_nickName, Server::_hostname));
			sendRepance(RPL_YOURHOST(this->_nickName, Server::_hostname));
			sendRepance(RPL_CREATED(this->_nickName, Server::_hostname));
			sendRepance(RPL_MYINFO(this->_nickName, Server::_hostname));
		}
		else
			sendRepance(ERR_NEEDMOREPARAMS(this->_nickName, Server::_hostname));
	}
	else
		sendRepance(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}
