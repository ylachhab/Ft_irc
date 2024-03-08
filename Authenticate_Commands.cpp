
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void Client::sendRepance(const std::string& msg)
{
	send(this->_fd, msg.c_str(), msg.size(), 0);
}

bool specialCharacter(std::string &str)
{
	std::string sp = "-[]\\'^{}";
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (!std::isalpha(*it) && !std::isdigit(*it) && sp.find(*it) == std::string::npos)
			return true;
	}
	return false;
}
/******************* PASS Command **********************/
void Client::executePass(std::vector<std::string> &vec)
{
	if (this->_registred)
		sendRepance(":yasmine 462 " + this->_nickName + " :You may not reregister\r\n");
	else
	{
		if (vec.size() && !vec[0].empty())
		{
			this->_pass = true;
			if (vec[0].compare(this->_password) == 0)
				this->_authenticated = true;
			else
				this->_authenticated = false;
		}
		else
			sendRepance(":yasmine  461 " + this->_nickName + " :Not enough parameters");
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
				sendRepance(":yasmin 433 " + this->_nickName + " " + vec[0] + " :Nickname is already in use\r\n");
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
				sendRepance(":yasmine 432 " + this->_nickName  + " " + vec[0] + " :Erroneous Nickname\r\n");
		}
		else
			sendRepance(":yasmine 431 " + this->_nickName  + " :No nickname given\r\n");
	}
	else
		sendRepance(":yasmine 451 " + this->_nickName + " :Register first(set the password)\r\n");
}

/******************* USER Command **********************/
void Client::executeUser(std::vector<std::string> &vec)
{
	if (this->_registred)
		sendRepance(":yasmine 462 " + this->_nickName + " :You may not reregister\r\n");
	if (this->_pass && this->_nick && this->_authenticated)
	{
		if (vec.size() >= 4)
		{
			if(specialCharacter(vec[0]) == 0)
			{
				this->_userName = vec[0];
				this->_realName = vec[3];
				this->_user = true;
				this->_registred = true;
				std::cout << "the user " << this->_userName << " was successfully regestred ";
				if (this->_authenticated)
					std::cout << "and authenticated!\n";
				else
					std::cout << "but not authenticated!\n";
			}
			else
				sendRepance(":yasmine 468 " + this->_nickName + " :Your username is invalid.\r\n");
		}
		else
			sendRepance(":yasmine 461 " + this->_nickName + " USER :Not enough parameters\r\n");
	}
	else
		sendRepance(":yasmine 451 " + this->_nickName + " :Register first(Set the password and a nickname)\r\n");
}
