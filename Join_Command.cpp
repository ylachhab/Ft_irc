#include "Client.hpp"


/******************* JOIN Command **********************/
std::vector<std::pair<std::string, std::string> > splitChannels
	(std::string& channels_Name, std::string& keys, size_t size)
{
	std::vector<std::pair<std::string, std::string> > vec;
	std::stringstream split_channel;
	std::string channel;
	split_channel << channels_Name;
	if (size < 2)
	{
		while (std::getline(split_channel, channel, ','))
			vec.push_back(std::make_pair(channel, ""));
	}
	else
	{
		std::stringstream split_keys;
		std::string key;
		split_keys << keys;
		while (std::getline(split_channel, channel, ',') && std::getline(split_keys, key, ','))
			vec.push_back(std::make_pair(channel, key));
	}
	return vec;
}

std::string removeExtraChar(const std::string& input, char del) {
	std::string result;
	bool charFlag = false;

	for (size_t i = 0; i < input.size(); ++i) {
		if (input[i] == del) {
			if (!charFlag) {
				result += del;
				charFlag = true;
			}
		} else {
			result += input[i];
			charFlag = false;
		}
	}
	if (!result.empty() && result[result.size() - 1] == del)
		result.erase(result.size() - 1);
	return result;
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

void Client::addNewChannel(std::string channelName)
{
	Channel channel;
	channel.setChannelName(channelName);
	channel.setOperator(this->_fd, this->getNickName());
	channel.getChannel().push_back(*this);
	Server::_channels.push_back(channel);
}

void Client::addToExistChannel(int index, std::string channelName)
{
	Server::_channels[index].getChannel().push_back(*this);
	std::string clients = Server::concatenateClients(Server::_channels[index]);
	sendRepance(RPL_JOIN(this->_nickName, this->_userName, channelName, Server::_ipaddress));
	sendRepance(RPL_NAMREPLY(Server::_hostname, clients, "#" + channelName, this->_nickName));
	sendRepance(RPL_ENDOFNAMES(Server::_hostname, this->_nickName, "#" + channelName));
}

void Client::executeJoin(std::vector<std::string> &vec)
{
	std::string tmp = removeExtraChar(vec[0], ',');
	if (this->_registred)
	{
		std::vector<std::pair<std::string, std::string> > v = splitChannels(tmp, vec[1], vec.size());
		for (std::vector<std::pair<std::string, std::string> >::iterator it = v.begin(); it != v.end() ; it++)
		{
			if (it->first[0] == '#' && it->first.length() > 2)
			{
				std::string channelName = it->first.substr(1);
				int index = existChannel(channelName);
				if (index != -1)
				{
					if (Server::_channels[index]._channelMode._inviteOnly)
					{
						if(isInvited(Server::_channels[index].getChannelName()))
							addToExistChannel(index, channelName);
						else
							sendRepance(ERR_INVITEONLY(this->_nickName, it->first));
					}
					else if (Server::_channels[index]._channelMode._key && Server::_channels[index]._channelMode._limit)
					{
						if (Server::_channels[index].getChannel().size() < static_cast<size_t>(Server::_channels[index].getlimitMbr()))
						{
							if(it->second == Server::_channels[index].getKey())
								addToExistChannel(index, channelName);
							else
								sendRepance(ERR_BADCHANNELKEY(this->_nickName, Server::_hostname, it->first));
						}
						else
							sendRepance(ERR_CHANNELISFULL(this->_nickName, it->first));
					}
					else if (Server::_channels[index]._channelMode._limit)
					{
						if (Server::_channels[index].getChannel().size() < static_cast<size_t>(Server::_channels[index].getlimitMbr()))
							addToExistChannel(index, channelName);
						else
							sendRepance(ERR_CHANNELISFULL(this->_nickName, it->first));
					}
					else if (Server::_channels[index]._channelMode._key)
					{
						if(it->second == Server::_channels[index].getKey())
							addToExistChannel(index, channelName);
						else
							sendRepance(ERR_BADCHANNELKEY(this->_nickName, Server::_hostname, it->first));
					}
					else if (Server::_channels[index]._channelMode.IKLoff() && !Server::isMember(Server::_channels[index].getChannelName(), this->getNickName()))
						addToExistChannel(index, channelName);
				}
				else if (index == -1)
				{
					addNewChannel(channelName);
					sendRepance(RPL_JOIN(this->_nickName, this->_userName, "#" + channelName, Server::_ipaddress));
					sendRepance(RPL_ENDOFNAMES(Server::_hostname, this->_nickName, "#" + channelName));
					sendRepance(RPL_NAMREPLY(Server::_hostname, "@" + this->_nickName, "#" + channelName,  this->_nickName));
				}

			}
			else
				sendRepance(ERR_NOSUCHCHANNEL(Server::_hostname, it->first, this->_nickName));
		}
		v.clear();
	}
	else
		sendRepance(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}