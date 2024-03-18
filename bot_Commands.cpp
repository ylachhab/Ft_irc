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
	sendRepance(std::string(BLUE) + "User informations : " + std::string(RESET) + "\r\n");
	sendRepance(std::string(BLUE) + "----------------------------" + std::string(RESET) + "\r\n");
	sendRepance(std::string(BLUE) + "Nickname : " + std::string(RESET) + "" + client.getNickName() + "\r\n");
	sendRepance(std::string(BLUE) + "Username : " + std::string(RESET) + "" + client.getUserName() + "\r\n");
	sendRepance(std::string(BLUE) + "realName : " + std::string(RESET) + "" + client.getRealName() + "\r\n");
	sendRepance(std::string(BLUE) + "----------------------------" + std::string(RESET) + "\r\n");
	sendRepance(std::string(CYAN)+ "Channels : " + std::string(RESET) + "\r\n");
	sendRepance(std::string(CYAN)+ "----------------------------" + std::string(RESET) + "\r\n");
	for (size_t i = 0; i < Server::_channels.size(); i++)
	{
		if (Server::_channels[i].isAMember(client.getNickName()))
		{
			sendRepance(std::string(CYAN)+ "-> " + Server::_channels[i].getChannelName() + "" + std::string(RESET) + "");
			if (Server::_channels[i].isOperator(client.getNickName()) != -1)
				sendRepance(std::string(CYAN)+ " *" + std::string(RESET) + "");
			sendRepance("\r\n");
			c++;
		}
	}
	if (c == 0)
		sendRepance(std::string(RED)+ "-> there is no channel!!" + std::string(RESET) + "\r\n");
	sendRepance (std::string(CYAN)+ "----------------------------" + std::string(RESET) + "\r\n");
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
					sendRepance (std::string(BLUE) + "Channel informations of " + tmpVec[i] + "" + std::string(RESET) + "\r\n");
					sendRepance (std::string(BLUE) + "----------------------------" + std::string(RESET) + "\r\n");
					sendRepance(std::string(BLUE) + "channel name : " + std::string(RESET) + "" + Server::_channels[index].getChannelName() + "\r\n");
					sendRepance(std::string(BLUE) + "channel topic : " + std::string(RESET) + "");
					if(Server::_channels[index].getChannelTopic().empty())
						sendRepance(std::string(RED)+ "the Topic not set!" + std::string(RESET) + "\r\n");
					else
						sendRepance(Server::_channels[index].getChannelTopic() + "\r\n");
					if (Server::_channels[index]._channelMode._limit)
						sendRepance(std::string(BLUE) + "Channel limit is : " + std::string(RESET) + "" + std::to_string(Server::_channels[index].getlimitMbr()) + "\r\n");
					std::vector<Client> vec_clients = Server::_channels[index].getChannel();
					sendRepance(std::string(BLUE) + "Channel Members : " + std::string(RESET) + "\r\n");
					for (size_t i = 0; i < vec_clients.size(); i++)
					{
						sendRepance(std::string(BLUE) + "- " + vec_clients[i].getNickName() + "" + std::string(RESET) + "");
						if (vec_clients[i].getNickName() == this->_nickName)
							sendRepance(std::string(RED) + "(Me)" + std::string(RESET) + "");
						sendRepance("\r\n");
					}
					sendRepance (std::string(BLUE) + "----------------------------" + std::string(RESET) + "\r\n");
				}
				else
					sendRepance(ERR_USERNOTINCHANNEL(Server::_hostname, Server::_channels[index].getChannelName()));
			}
			else
				sendRepance(ERR_NOSUCHCHANNEL(Server::_hostname, tmpVec[i], this->_nickName));
		}
		else
			sendRepance(ERR_NOSUCHCHANNEL(Server::_hostname, tmpVec[i], this->_nickName));
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
			{
				if (vec[vec.size() - 1].empty())
					sendRepance(ERR_NOTEXTTOSEND(Server::_hostname, this->_nickName));
				else
					sendClients(vec[vec.size() - 1] + "\r\n", "");
			}
		}

	}
}
