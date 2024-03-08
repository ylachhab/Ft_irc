#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

/******************* JOIN Command **********************/
std::vector<std::pair<std::string, std::string>> splitChannels()
{
	
}


int existChannel(std::string channelName)
{
	for (size_t i = 0; i < Server::_channels.size(); i++)
	{
		if (Server::_channels[i].getChannelName() == channelName)
			return i;
	}
	return -1;
}

bool ClientExistInChannel(Client *client)
{
	for (size_t i = 0; i < Server::_channels.size(); i++)
	{
		if (Server::_channels[i].isAMember(client->getNickName()))
			return true;
	}
	return false;
}
void Client::executeJoin(std::vector<std::string> &vec)
{
	if (this->_registred)
	{
		if (vec[0][0] == '#' || vec[0][0] == '&')
		{
			std::string channelName = vec[0].substr(1, vec[0].size() - 1);
			int index;
			index = existChannel(channelName);
			if(index != -1 && !ClientExistInChannel(this))
			{
				Server::_channels[index].getChannel().push_back(*this);
			}
			else if (index == -1)
			{
				Channel channel;
				channel.setChannelName(channelName);
				channel.setOperator(this->_fd);
				channel.getChannel().push_back(*this);
				Server::_channels.push_back(channel);
			}
			// else
			// {
			// 	for (size_t i = 0; i < Server::_channels.size(); i++)
			// 	{
			// 		puts("----------------------------------------------");
			// 		std::cout << "exist channel: " << Server::_channels[i].getChannelName() << "\n";
			// 		for (size_t i = 0; i < Server::_channels[i].getChannel().size(); i++)
			// 		{
			// 			std::cout << Server::_channels[i].getChannel()[i].getNickName() << " ";
			// 		}
			// 		std::cout << std::endl;
			// 		puts("----------------------------------------------");
			// 	}
			// }
		}
		else
			sendRepance(":yasmine 403 " +  this->_nickName  + " " + vec[0] + " :No such channel\r\n");
	}
	else
		sendRepance(":yasmine 451 " + this->_nickName + " :Register first.\r\n");
}