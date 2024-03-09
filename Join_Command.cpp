#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

/******************* JOIN Command **********************/
std::vector<std::pair<std::string, std::string> > splitChannels
	(std::string& channels_Name, std::string& keys, size_t size)
{
	std::vector<std::pair<std::string, std::string> > vec;
	std::stringstream split_channel;
	std::stringstream split_keys;
	std::string channel;
	std::string key;
	split_channel << channels_Name;
	split_keys << keys;
	if (size < 2)
	{
		while (std::getline(split_channel, channel, ','))
			vec.push_back(std::make_pair(channel, ""));
	}
	while (std::getline(split_channel, channel, ',') && std::getline(split_keys, key, ','))
		vec.push_back(std::make_pair(channel, key));
	return vec;
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

void Client::addNewChannel(std::string channelName)
{
	Channel channel;
	channel.setChannelName(channelName);
	channel.setOperator(this->_fd, this->getNickName());
	channel.getChannel().push_back(*this);
	Server::_channels.push_back(channel);
	int num = this->getMaxChannel();
	this->setMaxChannel(num++);
}

void Client::executeJoin(std::vector<std::string> &vec)
{
	std::string tmp;
	if (this->_registred)
	{
		std::vector<std::pair<std::string, std::string> > v = splitChannels(vec[0], vec[1], vec.size());
		for (std::vector<std::pair<std::string, std::string> >::iterator it = v.begin(); it != v.end(); it++)
		{
			if (it->first[0] == '#' || it->first[0] == '&')
			{
				std::string channelName = it->first.substr(1, it->first.size() - 1);
				int index;
				index = existChannel(channelName);
				if(index != -1 && !ClientExistInChannel(this) && this->getMaxChannel() < 10)
				{
					int num = this->getMaxChannel();
					this->setMaxChannel(num++);
					// if (Server::_channels[index]._passSet &&  Server::_channels[index].getChannelPassword() == it->second)
					Server::_channels[index].getChannel().push_back(*this);
					// else if (!Server::_channels[index]._passSet)
					// 	Server::_channels[index].getChannel().push_back(*this);
					// else
					// 	sendRepance(":yasmine 403 " +  this->_nickName  + " " + vec[0] + " :invalid password \r\n");
				}
				else if (index == -1)
					addNewChannel(channelName);
			}
			else
				sendRepance(":yasmine 403 " +  this->_nickName  + " " + vec[0] + " :No such channel\r\n");
		}
		v.clear();
	}
	else
		sendRepance(":yasmine 451 " + this->_nickName + " :Register first.\r\n");
}