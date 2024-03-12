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
	channel._channelMode._key = true;
	channel.setChannelPwd("hello");
	channel.setChannelName(channelName);
	channel.setOperator(this->_fd, this->getNickName());
	channel.getChannel().push_back(*this);
	Server::_channels.push_back(channel);
}

bool Client::checkMods(Channel &channel, std::vector<std::pair<std::string, std::string> >::iterator it)
{
	std::vector<std::string>::iterator itr = std::find(channelInvite.begin(), channelInvite.end(), channel.getChannelName());
	if (channel._channelMode._inviteOnly && itr != channelInvite.end())
		return true;
	else if (channel._channelMode._key && it->second == channel.getChannelPwd())
		return true;
	else if (channel._channelMode._limit && channel.getChannel().size() < static_cast<size_t>(channel.getlimitMbr()))
		return true;
	else if (channel._channelMode.IKLoff() && !Server::isMember(channel.getChannelName(), this->getNickName()))
	{
		std::cout << channel._channelMode.IKLoff() << Server::isMember(channel.getChannelName(), this->getNickName()) << "\n";
		return true;
	}
	return false;
}

void Client::executeJoin(std::vector<std::string> &vec)
{
	std::string tmp = removeExtraChar(vec[0], ',');
	if (this->_registred)
	{
		std::vector<std::pair<std::string, std::string> > v = splitChannels(tmp, vec[1], vec.size());
		for (std::vector<std::pair<std::string, std::string> >::iterator it = v.begin(); it != v.end() ; it++)
		{
			if (it->first[0] == '#')
			{
				std::string channelName = it->first;
				int index;
				index = existChannel(channelName);
				if(index != -1 && checkMods(Server::_channels[index], it))
				{
					Server::_channels[index].getChannel().push_back(*this);
					std::string clients = Server::concatenateClients(Server::_channels[index]);
					sendRepance(RPL_NAMREPLY(Server::_hostname, clients, channelName, this->_nickName));
					sendRepance(RPL_ENDOFNAMES(Server::_hostname, this->_nickName, channelName));
				}
				else if (index == -1)
				{
					addNewChannel(channelName);
					sendRepance(RPL_NAMREPLY(Server::_hostname, "@" + this->_nickName, channelName,  this->_nickName));
					sendRepance(RPL_ENDOFNAMES(Server::_hostname, this->_nickName, channelName));
				}
				else
					std::cout << "error in channel\n";
			}
			else
				sendRepance(ERR_NOSUCHCHANNEL(Server::_hostname, it->first, this->_nickName));
		}
		v.clear();
	}
	else
		sendRepance(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
}