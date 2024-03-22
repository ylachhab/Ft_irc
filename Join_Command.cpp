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
		while (std::getline(split_channel, channel, ','))
		{
			if(std::getline(split_keys, key, ','))
				vec_pair.push_back(std::make_pair(channel, key));
			else
				vec_pair.push_back(std::make_pair(channel, ""));

		}
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
int Client::existChannel(std::string channelName)
{
	for (size_t i = 0; i < Server::_channels.size(); i++)
	{
		if (to_Upper(Server::_channels[i].getChannelName()) == channelName)
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
	if (!Server::_channels[index].getChannelTopic().empty())
		sendTo(RPL_TOPIC(this->_nickName, Server::_hostname, "#" + channelName, Server::_channels[index].getChannelTopic()));
	sendTo(RPL_JOIN(this->_nickName, this->_userName, "#" + channelName, this->clientIp));
	sendTo(RPL_NAMREPLY(Server::_hostname, clients, "#" + channelName, this->_nickName));
	sendTo(RPL_ENDOFNAMES(Server::_hostname, this->_nickName, "#" + channelName));
	std::vector<Client> vec_client = Server::_channels[index].getChannel();
	for (size_t i = 0; i < vec_client.size(); i++)
	{
		if (vec_client[i].getNickName() != this->_nickName)
		{
			std::string msg = RPL_JOIN(this->_nickName, this->_userName, "#" + channelName, this->clientIp);
			send(vec_client[i].getFd(), msg.c_str(), msg.length(), 0);
		}
	}
}

void Client::executeJoin()
{
	if (this->_registred)
	{
		if (vec.size())
		{
			std::string tmp = removeExtraChar(vec[0], ',');
			std::vector<std::pair<std::string, std::string> > v = splitChannels(tmp);
			for (std::vector<std::pair<std::string, std::string> >::iterator it = v.begin(); it != v.end() ; it++)
			{
				if (it->first[0] == '#' && it->first.length() > 1)
				{
					std::string channelName = it->first.substr(1);
					int index = existChannel(to_Upper(channelName));
					if (index != -1 && !Server::isMember(Server::_channels[index].getChannelName(), this->getNickName()))
					{
						if (Server::_channels[index]._channelMode._inviteOnly)
						{
							if(isInvited(Server::_channels[index].getChannelName()))
								addToExistChannel(index, channelName);
							else
								sendTo(ERR_INVITEONLY(this->_nickName, it->first));
						}
						else if (Server::_channels[index]._channelMode._key && Server::_channels[index]._channelMode._limit)
						{
							if (Server::_channels[index].getChannel().size() < static_cast<size_t>(Server::_channels[index].getlimitMbr()))
							{
								if(it->second == Server::_channels[index].getKey())
									addToExistChannel(index, channelName);
								else
									sendTo(ERR_BADCHANNELKEY(this->_nickName, Server::_hostname, it->first));
							}
							else
								sendTo(ERR_CHANNELISFULL(this->_nickName, it->first));
						}
						else if (Server::_channels[index]._channelMode._limit)
						{
							if (Server::_channels[index].getChannel().size() < static_cast<size_t>(Server::_channels[index].getlimitMbr()))
								addToExistChannel(index, channelName);
							else
								sendTo(ERR_CHANNELISFULL(this->_nickName, it->first));
						}
						else if (Server::_channels[index]._channelMode._key)
						{
							if(it->second == Server::_channels[index].getKey())
								addToExistChannel(index, channelName);
							else
								sendTo(ERR_BADCHANNELKEY(this->_nickName, Server::_hostname, it->first));
						}
						else if (Server::_channels[index]._channelMode.IKLoff())
							addToExistChannel(index, channelName);
					}
					else if (index == -1)
					{
						addNewChannel(channelName);
						sendTo(RPL_JOIN(this->_nickName, this->_userName, "#" + channelName, this->clientIp));
						sendTo(RPL_NAMREPLY(Server::_hostname, "@" + this->_nickName, "#" + channelName,  this->_nickName));
						sendTo(RPL_ENDOFNAMES(Server::_hostname, this->_nickName, "#" + channelName));
					}
				}
				else
					sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, it->first, this->_nickName));
			}
			v.clear();
		}
		else
			sendTo(ERR_NEEDMOREPARAMS(this->_nickName, Server::_hostname));
	}
	else
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}