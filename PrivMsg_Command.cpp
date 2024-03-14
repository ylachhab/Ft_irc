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

void Client::executePrivMsg()
{
	if (this->_registred)
	{
		std::string tmp = removeExtraChar(vec[0], ',');
		t_members member = splitMembers(tmp);
		std::string str;
		for (size_t i = 0; i < member.vec_channels.size(); i++)
		{
			str = member.vec_channels[i].substr(1);
			if (str.size() < 1)
				sendRepance(ERR_NOSUCHCHANNEL(Server::_hostname, member.vec_channels[i], this->_nickName));
			int index = Server::retChannel(str);
			if (index != -1)
			{
				std::vector<Client> clients = Server::_channels[index].getChannel();
				for (size_t j = 0; j < clients.size(); j++)
				{
					if (Server::_channels[index].isOperator(clients[j].getNickName()) == -1)
					{
						std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + Server::_ipaddress + " PRIVMSG " + clients[j].getNickName() + " :" + vec[1] + "\r\n";
						send(clients[j].getFd(), msg.c_str(), msg.length(), 0);
					}
				}
			}
			else
				sendRepance(ERR_NOSUCHCHANNEL(Server::_hostname, member.vec_channels[i], this->_nickName));
		}
		for (size_t i = 0; i < member.vec_clients.size(); i++)
		{
			int index = Server::retClient(member.vec_clients[i]);
			if (index != -1)
			{
				std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + Server::_ipaddress + " PRIVMSG " + Server::cObjs[index].getNickName() + " :" + vec[1] + "\r\n";
				send(Server::cObjs[index].getFd(), msg.c_str(), msg.length(), 0);
			}
			else
				sendRepance(Server::_hostname + " 401 " + this->_nickName + " " + member.vec_clients[i] + " :No such nick\r\n");
		}
	}
	else
		sendRepance(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}