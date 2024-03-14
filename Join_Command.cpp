#include "Client.hpp"

/******************* utils **********************/
std::vector<std::pair<std::string, std::string> > Client::splitChannels
	(std::string& channels_Name)
{
	std::vector<std::pair<std::string, std::string> > vec_pair;
	std::stringstream split_channel;
	std::string channel;
	split_channel << channels_Name;
	if (vec.size() < 2)
	{
		while (std::getline(split_channel, channel, ','))
			vec_pair.push_back(std::make_pair(channel, ""));
	}
	else
	{
		std::stringstream split_keys;
		std::string key;
		split_keys << vec[1];
		while (std::getline(split_channel, channel, ',') && std::getline(split_keys, key, ','))
			vec_pair.push_back(std::make_pair(channel, key));
	}
	return vec_pair;
}

std::string Client::removeExtraChar(const std::string& input, char del) {
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

/******************* JOIN Command **********************/
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
	sendRepance(RPL_JOIN(this->_nickName, this->_userName, channelName, this->clientIp));
	sendRepance(RPL_NAMREPLY(Server::_hostname, clients, "#" + channelName, this->_nickName));
	sendRepance(RPL_ENDOFNAMES(Server::_hostname, this->_nickName, "#" + channelName));
	std::map<int, std::string> opers =  Server::_channels[index].getOperator();
	for (std::map<int, std::string>::iterator it = opers.begin(); it != opers.end(); it++)
	{
		std::string msg = RPL_JOIN(this->_nickName, this->_userName, channelName, this->clientIp);
		send (it->first, msg.c_str(), msg.length(), 0);
	}
}

void Client::executeJoin()
{
	if (this->_registred)
	{
		std::string tmp = removeExtraChar(vec[0], ',');
		std::vector<std::pair<std::string, std::string> > v = splitChannels(tmp);
		for (std::vector<std::pair<std::string, std::string> >::iterator it = v.begin(); it != v.end() ; it++)
		{
			if (it->first[0] == '#' && it->first.length() > 1)
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
					sendRepance(RPL_JOIN(this->_nickName, this->_userName, "#" + channelName, this->clientIp));
					sendRepance(RPL_NAMREPLY(Server::_hostname, "@" + this->_nickName, "#" + channelName,  this->_nickName));
					sendRepance(RPL_ENDOFNAMES(Server::_hostname, this->_nickName, "#" + channelName));
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