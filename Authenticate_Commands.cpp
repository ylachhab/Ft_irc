
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

/******************* Utils Functions **********************/

bool specialCharacter(std::string &str)
{
	std::string sp = "-[]\\'^{}_";
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

void Client::isRegesterd()
{
	if (this->_nick && this->_user && !this->_registred)
	{
		this->_registred = true;
		sendTo(RPL_WELCOME(this->_nickName, Server::_hostname));
		sendTo(RPL_YOURHOST(this->_nickName, Server::_hostname));
		sendTo(RPL_CREATED(this->_nickName, Server::_hostname));
		sendTo(RPL_MYINFO(this->_nickName, Server::_hostname));
	}
}

void Client::sendToAll(std::string msg, std::string channel) {
	for (size_t i = 0; i < Server::cObjs.size(); i++)
	{
		if (Server::isMember(channel, Server::cObjs[i].getNickName()) 
			&& Server::cObjs[i].getNickName() != this->_nickName && !Server::cObjs[i].sendMsg)
		{
			send(Server::cObjs[i].getFd(), msg.c_str(), msg.length(), 0);
			Server::cObjs[i].sendMsg = true;
		}
	}
}
/******************* PASS Commande **********************/
void Client::executePass()
{
	if (this->_registred)
		sendTo(ERR_ALREADYREGISTERED(this->_nickName, Server::_hostname));
	else
	{
		if (vec.size() != 0 && !vec[0].empty())
		{
			this->_pass = true;
			if (vec[0].compare(this->_password) == 0)
				this->_authenticated = true;
			else
			{
				this->_authenticated = false;
				sendTo(ERR_PASSWDMISMATCH(this->_nickName, Server::_hostname));
			}
		}
		else
			sendTo(ERR_NEEDMOREPARAMS(this->_nickName, Server::_hostname));
	}
}


/******************* NICK Command **********************/
void Client::executeNick()
{
	if (this->_pass && this->_authenticated)
	{
		if (vec.size() && !vec[0].empty())
		{
			if (specialCharacter(vec[0]) == 0)
			{
				for (size_t i = 0; i < Server::cObjs.size(); i++)
				{
					if (Server::cObjs[i].getFd() != this->_fd
						&& Server::cObjs[i]._nickName == vec[0])
					{
						sendTo(ERR_NICKNAMEINUSE(vec[0], this->_nickName, Server::_hostname));
						this->_nick = false;
						return ;
					}
				}
				this->_nick = true;
				vec[0] = vec[0].substr(0, 15);
				if (!this->_registred)
				{
					if(this->_nickName != "*")
						sendTo(":" + this->_nickName + "!" + this->_userName + "@" + " NICK " + ":" + vec[0] + "\r\n");
					this->_nickName = vec[0];
				}
				else
				{
					std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " NICK " + ":" + vec[0] + "\r\n";
					for (size_t i = 0; i < Server::_channels.size(); i++)
					{
						if (Server::isMember(Server::_channels[i].getChannelName(), this->_nickName))
						{
							sendToAll(msg, Server::_channels[i].getChannelName());
							if (Server::_channels[i].isOperator(this->_nickName) != -1)
							{
								Server::_channels[i].getOperator().erase(this->_fd);
								Server::_channels[i].setOperator(this->_fd, vec[0]);
							}
							int index = Server::retChannelMember(this->_nickName, i);
							Server::_channels[i].getChannel()[index].setNickName(vec[0]);
						}
					}
					sendTo(msg);
					Server::setAllSendOff();
					this->_nickName = vec[0];
				}
				isRegesterd();
			}
			else
				sendTo(ERR_ERRONEUSNICKNAME(this->_nickName, Server::_hostname));
		}
		else
			sendTo(ERR_NONICKNAMEGIVEN(this->_nickName, Server::_hostname));
	}
	else
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}

/******************* USER Command **********************/
void Client::executeUser()
{
	if (this->_registred)
		sendTo(ERR_ALREADYREGISTERED(this->_nickName, Server::_hostname));
	else if (this->_pass && this->_authenticated)
	{
		if (vec.size() >= 4 && !vec[0].empty() && !vec[3].empty()
			&& Server::IsValidChar(vec[0]) && Server::IsValidChar(vec[3]))
		{
			vec[0] = vec[0].substr(0, 10);
			vec[3] = vec[3].substr(0, 20);
			this->_userName = vec[0];
			this->_realName = vec[3];
			this->_user = true;
			isRegesterd();
		}
		else
			sendTo(ERR_NEEDMOREPARAMS(this->_nickName, Server::_hostname));
	}
	else
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}


void Client::executeQuit()
{
	std::cout << "pollserver: socket " << this->_fd <<" hung up\n";
	close(this->_fd);
	for (size_t j = 0; j < Server::_channels.size(); j++)
	{
		if (Server::_channels[j].isAMember(this->_nickName))
		{
			std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " QUIT " + ":Quit\r\n";
			sendToAll(msg, Server::_channels[j].getChannelName());
		}
	}
	Server::setAllSendOff();
	error = true;
}
