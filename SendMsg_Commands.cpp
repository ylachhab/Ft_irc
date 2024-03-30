#include "Client.hpp"

typedef struct membersToSendMsg
{
	std::vector<std::string> vec_clients;
	std::vector<std::string> vec_channels;
} t_members;

t_members splitMembers(std::string& members)
{
	t_members memb;
	std::stringstream split_clients(members);
	std::string member;
	while (std::getline(split_clients, member, ','))
	{
		if (!member.empty() && member[0] != '#' && std::find(memb.vec_clients.begin(),
			memb.vec_clients.end(), member) == memb.vec_clients.end())
			memb.vec_clients.push_back(member);
		else if (!member.empty() && member[0] == '#' && std::find(memb.vec_channels.begin(),
			memb.vec_channels.end(), member) == memb.vec_channels.end())
			memb.vec_channels.push_back(member);
	}
	return memb;
}

/****************** PRIVMSG Command *******************/

void Client::executePrivMsg()
{
	if (this->_registred)
	{
		if (!vec.size() || vec[0].empty())
		{
			sendTo(ERR_NORECIPIENT(this->_nickName, Server::_hostname, std::string("PRIVMSG")));
			return;
		}
		std::string tmp = removeExtraChar(vec[0], ',');
		t_members member = splitMembers(tmp);
		std::string str;
		if (vec.size() < 2 || vec[vec.size() - 1].empty())
		{
			sendTo(ERR_NOTEXTTOSEND(Server::_hostname, this->_nickName));
			return;
		}
		for (size_t i = 0; i < member.vec_channels.size(); i++)
		{
			str = member.vec_channels[i].substr(1);
			if (str.size() < 1)
			{
				sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, member.vec_channels[i], this->_nickName));
				return;
			}
			int index = Server::retChannel(str);
			if (index != -1)
			{
				if (Server::_channels[index].isAMember(this->_nickName))
				{
					std::vector<Client> clients = Server::_channels[index].getChannel();

					for (size_t j = 0; j < clients.size(); j++)
					{
						if (clients[j]._nickName != this->_nickName)
						{
							std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " PRIVMSG " + "#" + Server::_channels[index].getChannelName() + " :" + vec[vec.size() - 1] + "\r\n";
							send(clients[j].getFd(), msg.c_str(), msg.length(), 0);
						}
					}
				}
				else
					sendTo(ERR_CANNOTSENDTOCHAN(Server::_hostname, member.vec_channels[i], this->_nickName));
			}
			else
				sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, member.vec_channels[i], this->_nickName));
		}
		for (size_t i = 0; i < member.vec_clients.size(); i++)
		{
			if (to_Upper(member.vec_clients[i]) == "BOT" && Server::botSet != -1)
				executeBot();
			else
			{
				int index = Server::retClient(member.vec_clients[i]);
				if (index != -1)
				{
					if (vec[vec.size() - 1].empty())
						sendTo(ERR_NOTEXTTOSEND(Server::_hostname, this->_nickName));
					else
					{
						std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " PRIVMSG " + Server::cObjs[index].getNickName() + " :" + vec[vec.size() - 1] + "\r\n";
						send(Server::cObjs[index].getFd(), msg.c_str(), msg.length(), 0);
					}
				}
				else
					sendTo(":" + Server::_hostname + " 401 " + this->_nickName + " " + member.vec_clients[i] + " :No such nick\r\n");
			}
		}
	}
	else
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}

/****************** NOTICE Command *******************/

void Client::executeNotice()
{
	if (this->_registred)
	{
		if (!vec.size() || vec[0].empty())
		{
			sendTo(ERR_NORECIPIENT(this->_nickName, Server::_hostname, std::string("NOTICE")));
			return;
		}
		std::string tmp = removeExtraChar(vec[0], ',');
		t_members member = splitMembers(tmp);
		std::string str;
		if (vec.size() < 2 ||  vec[vec.size() - 1].empty())
		{
			sendTo(ERR_NOTEXTTOSEND(Server::_hostname, this->_nickName));
			return;
		}
		for (size_t i = 0; i < member.vec_channels.size(); i++)
		{
			str = member.vec_channels[i].substr(1);
			if (str.size() < 1)
				return;
			int index = Server::retChannel(str);
			if (index != -1)
			{
				if (Server::_channels[index].isAMember(this->_nickName))
				{
					std::vector<Client> clients = Server::_channels[index].getChannel();

					for (size_t j = 0; j < clients.size(); j++)
					{
						if (clients[j]._nickName != this->_nickName)
						{
							std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " NOTICE " + "#" + Server::_channels[index].getChannelName() + " :" + vec[vec.size() - 1] + "\r\n";
							send(clients[j].getFd(), msg.c_str(), msg.length(), 0);
						}
					}
				}
			}
		}
		for (size_t i = 0; i < member.vec_clients.size(); i++)
		{
			int index = Server::retClient(member.vec_clients[i]);
			if (index != -1)
			{
				if (vec[vec.size() - 1].empty())
					sendTo(ERR_NOTEXTTOSEND(Server::_hostname, this->_nickName));
				else
				{
					std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " NOTICE " + this->_nickName + " :" + vec[vec.size() - 1] + "\r\n";
					send(Server::cObjs[index].getFd(), msg.c_str(), msg.length(), 0);
				}
			}
		}
	}
}