#include "Client.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

std::vector<std::string> splitMsg(std::string str)
{
	std::stringstream split_clients(str);
	std::vector<std::string> tmp_vec;
	std::string msg;
	while (std::getline(split_clients, msg, ','))
		tmp_vec.push_back(msg);
	return tmp_vec;
}

void Client::executeBotClient(Client &client)
{
	int c = 0;
	sendTo(std::string(BLUE) + "User informations : " + std::string(RESET) + "\r\n");
	sendTo(std::string(BLUE) + "----------------------------" + std::string(RESET) + "\r\n");
	sendTo(std::string(BLUE) + "Nickname : " + std::string(RESET) + "" + client.getNickName() + "\r\n");
	sendTo(std::string(BLUE) + "Username : " + std::string(RESET) + "" + client.getUserName() + "\r\n");
	sendTo(std::string(BLUE) + "realName : " + std::string(RESET) + "" + client.getRealName() + "\r\n");
	sendTo(std::string(BLUE) + "----------------------------" + std::string(RESET) + "\r\n");
	sendTo(std::string(CYAN)+ "Channels : " + std::string(RESET) + "\r\n");
	sendTo(std::string(CYAN)+ "----------------------------" + std::string(RESET) + "\r\n");
	for (size_t i = 0; i < Server::_channels.size(); i++)
	{
		if (Server::_channels[i].isAMember(client.getNickName()))
		{
			sendTo(std::string(CYAN)+ "-> " + Server::_channels[i].getChannelName() + "" + std::string(RESET) + "");
			if (Server::_channels[i].isOperator(client.getNickName()) != -1)
				sendTo(std::string(CYAN)+ " *" + std::string(RESET) + "");
			sendTo("\r\n");
			c++;
		}
	}
	if (c == 0)
		sendTo(std::string(RED)+ "-> there is no channel!!" + std::string(RESET) + "\r\n");
	sendTo (std::string(CYAN)+ "----------------------------" + std::string(RESET) + "\r\n");
}

void Client::executeBotChannel()
{
	std::string tmp = removeExtraChar(vec[0], ',');
	std::vector<std::string> tmpVec = splitMsg(vec[0]);
	for (size_t i = 0; i < tmpVec.size(); i++)
	{
		if (tmpVec[i][0] == '#')
		{
			int index = Server::retChannel(tmpVec[i].substr(1));
			if (index != -1)
			{
				if (Server::_channels[index].isAMember(this->_nickName))
				{
					sendTo (std::string(BLUE) + "Channel informations of " + tmpVec[i] + "" + std::string(RESET) + "\r\n");
					sendTo (std::string(BLUE) + "----------------------------" + std::string(RESET) + "\r\n");
					sendTo(std::string(BLUE) + "channel name : " + std::string(RESET) + "" + Server::_channels[index].getChannelName() + "\r\n");
					sendTo(std::string(BLUE) + "channel topic : " + std::string(RESET) + "");
					if(Server::_channels[index].getChannelTopic().empty())
						sendTo(std::string(RED)+ "the Topic not set!" + std::string(RESET) + "\r\n");
					else
						sendTo(Server::_channels[index].getChannelTopic() + "\r\n");
					if (Server::_channels[index]._channelMode._limit)
						sendTo(std::string(BLUE) + "Channel limit is : " + std::string(RESET) + "" + std::to_string(Server::_channels[index].getlimitMbr()) + "\r\n");
					std::vector<Client> vec_clients = Server::_channels[index].getChannel();
					sendTo(std::string(BLUE) + "Channel Members : " + std::string(RESET) + "\r\n");
					for (size_t i = 0; i < vec_clients.size(); i++)
					{
						sendTo(std::string(BLUE) + "- " + vec_clients[i].getNickName() + "" + std::string(RESET) + "");
						if (vec_clients[i].getNickName() == this->_nickName)
							sendTo(std::string(RED) + "(Me)" + std::string(RESET) + "");
						sendTo("\r\n");
					}
					sendTo (std::string(BLUE) + "----------------------------" + std::string(RESET) + "\r\n");
				}
				else
					sendTo(ERR_USERNOTINCHANNEL(Server::_hostname, Server::_channels[index].getChannelName()));
			}
			else
				sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, tmpVec[i], this->_nickName));
		}
		else
			sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, tmpVec[i], this->_nickName));
	}
}

void Client::executeBot()
{
	if (this->_registred)
	{
		if (vec.size())
		{
			int j;
			if (to_Upper(vec[0]) == "ME")
				executeBotClient(*this);
			else if (vec[0][0] == '#')
				executeBotChannel();
			else if ((j = Server::retClient(vec[0])) != -1)
				executeBotClient(Server::cObjs[j]);
			else
				sendTo(Server::_hostname + " 401 " + this->_nickName + " " + vec[0] + " :No such nick\r\n");
		}
		else
			sendTo(ERR_NEEDMOREPARAMS(this->_nickName, Server::_hostname));
	}
	else
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}

