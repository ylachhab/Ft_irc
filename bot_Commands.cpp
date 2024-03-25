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
	std::string msg = "User informations \r";
	sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, " User informations"));
	sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, " ----------------------------"));
	sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, "    Nickname -> " + client.getNickName()));
	sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, "    Username -> " + client.getUserName()));
	sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, "    realName -> " + client.getRealName()));
	sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, " ----------------------------"));
	sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, " Channels"));
	sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, " ----------------------------"));
	for (size_t i = 0; i < Server::_channels.size(); i++)
	{
		if (Server::_channels[i].isAMember(client.getNickName()))
		{
			if (Server::_channels[i].isOperator(client.getNickName()) != -1)
				sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, "    ->" + Server::_channels[i].getChannelName()+ " (operator)"));
			else
				sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, "    ->" + Server::_channels[i].getChannelName()));
			c++;
		}
	}
	if (c == 0)
		sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, " -> there is no channel!!"));
	sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, client._nickName, " ----------------------------"));
}

void Client::executeBotChannel()
{
	std::string tmp = removeExtraChar(vec[1], ',');
	std::vector<std::string> tmpVec = splitMsg(vec[1]);
	for (size_t i = 0; i < tmpVec.size(); i++)
	{
		if (tmpVec[i][0] == '#')
		{
			int index = Server::retChannel(tmpVec[i].substr(1));
			if (index != -1)
			{
				sendTo (RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName, " Channel informations of " + tmpVec[i]));
				sendTo (RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName, " ----------------------------"));
				sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName, "    channel name : " + Server::_channels[index].getChannelName()));
				if(Server::_channels[index].getChannelTopic().empty())
					sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName,"    channel topic : the Topic not set!"));
				else
					sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName, "    channel topic : " + Server::_channels[index].getChannelTopic()));
				if (Server::_channels[index]._channelMode._limit)
					sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName,"Channel limit is : " + std::to_string(Server::_channels[index].getlimitMbr())));
				std::vector<Client> vec_clients = Server::_channels[index].getChannel();
				sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName,"Channel Members : "));
				for (size_t i = 0; i < vec_clients.size(); i++)
				{
					if (vec_clients[i].getNickName() == this->_nickName)
						sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName,"- " + vec_clients[i].getNickName() + "(Me)"));
					else
						sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName,"- " + vec_clients[i].getNickName()));
				}
				sendTo (RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName,"----------------------------"));
				// }
				// else
				// 	sendTo(ERR_USERNOTINCHANNEL(Server::_hostname, Server::_channels[index].getChannelName()));
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
			if (to_Upper(vec[1]) == "ME")
				executeBotClient(*this);
			else if (vec[1][0] == '#')
				executeBotChannel();
			else if ((j = Server::retClient(vec[1])) != -1)
				executeBotClient(Server::cObjs[j]);
			else
				sendTo(Server::_hostname + " 401 " + this->_nickName + " " + vec[1] + " :No such nick\r\n");
		}
		else
			sendTo(ERR_NEEDMOREPARAMS(this->_nickName, Server::_hostname));
	}
	else
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}
void Client::Bot()
{
	if (this->_registred)
	{
		if (vec.empty())
		{
			if (Server::botSet == -1)
			{
				Server::botSet = this->_fd;
				sendTo(RPL_BOT(this->_nickName, this->_userName, this->clientIp, this->_nickName, " bot is set"));
			}
			else
				sendTo(ERR_BOT(Server::_hostname, this->_nickName));
		}
	}
	else
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}

