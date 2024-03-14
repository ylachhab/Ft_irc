#include "Client.hpp"

bool Client::checkError() {
	if (!this->_authenticated || !this->_registred)
	{
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
		// std::cout << " :Register first." << std::endl;
		return true ;
	}
	std::string str = vec[0];
	if (str[0] == '#' && str.length() > 1)
		vec[0] = vec[0].substr(1);
	else {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		// sendTo(this->_nickName + " " + vec[0] + " :No such channel" + "\r\n");
		return true;
	}
	if (!Server::findChannel(vec[0])) {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		// sendTo(this->_nickName + " " + str + " :No such channel" + "\r\n");
		return true;
	}
	if (Server::findOperator(vec[0], this->_nickName) != this->_fd) {
		sendTo(ERR_NOTOP(Server::_hostname, str));
		// sendTo(this->_nickName + " " + str + " :You're not channel operator" + "\r\n");
		return true;
	}
	return false;
}

void Client::checkFlag(std::string channel) {
	int i = Server::retChannel(channel);
	std::string sub;

	if (Server::_channels[i]._channelMode.allOff())
	{
		Server::_channels[i]._channelMode._topic = true;
		sub = "t";
	}
	else if (Server::_channels[i]._channelMode.allOn())
		sub = "tilk";
	else {
		if (Server::_channels[i]._channelMode._topic)
			sub += "t";
		if (Server::_channels[i]._channelMode._inviteOnly)
			sub += "i";
		if (Server::_channels[i]._channelMode._limit)
			sub += "l";
		if (Server::_channels[i]._channelMode._key)
			sub += "k";
	}
	if (Server::_channels[i]._channelMode._limit && Server::_channels[i]._channelMode._key)
	{
		std::stringstream ss(Server::_channels[i].getlimitMbr());
		std::string lim;
		ss >> lim;
		sendTo(":" + Server::_hostname + " 324 " + this->_nickName + " #" + channel + " +" + sub + " " + lim + " "
			+ Server::_channels[i].getKey() + "\r\n");
	}
	else if (Server::_channels[i]._channelMode._limit)
	{
		std::stringstream ss(Server::_channels[i].getlimitMbr());
		std::string lim;
		ss >> lim;
		sendTo(":" + Server::_hostname + " 324 " + this->_nickName + " #" + channel + " +" + sub + " " + lim + "\r\n");
	}
	else if (Server::_channels[i]._channelMode._key)
		sendTo(":" + Server::_hostname + " 324 " + this->_nickName + " #" + channel + " +" + sub + " "
			+ Server::_channels[i].getKey() + "\r\n");
	else
		sendTo(":" + Server::_hostname + " 324 " + this->_nickName + " #" + channel + " +" + sub + "\r\n");
}

void Client::checkTopic(char sign, int index, std::string channel) {
	if (!Server::_channels[index]._channelMode._topic && sign == '+')
	{
		Server::_channels[index]._channelMode._topic = true;
		sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE " + channel + " +t" + "\r\n");
		sendClients(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE "
			+ channel + " +t" + "\r\n", channel.substr(1));
	}
	if (Server::_channels[index]._channelMode._topic && sign == '-')
	{
		Server::_channels[index]._channelMode._topic = false;
		sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE " + channel + " -t" + "\r\n");
		sendClients(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE "
			+ channel + " -t" + "\r\n", channel.substr(1));
	}
}

void Client::checkInvite(char sign, int index, std::string channel) {
	if (!Server::_channels[index]._channelMode._inviteOnly && sign == '+')
	{
		Server::_channels[index]._channelMode._inviteOnly = true;
		// sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE " + channel + " +i" + "\r\n");
		sendClients(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE "
			+ channel + " +i" + "\r\n", channel.substr(1));
	}
	if (Server::_channels[index]._channelMode._inviteOnly && sign == '-')
	{
		Server::_channels[index]._channelMode._inviteOnly = false;
		// sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE " + channel + " -i" + "\r\n");
		sendClients(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE "
			+ channel + " -i" + "\r\n", channel.substr(1));
	}
}

void Client::checkOperatorFlag(char sign, int index, std::string channel, std::string arg) {
	if (!Server::existeNick(arg))
		sendTo(ERR_NOSUCHNICK(Server::_hostname, channel, arg));
	else if (!Server::isMember(vec[0], arg))
		sendTo(ERR_USERNOTINCHANNEL(Server::_hostname, vec[0]));
	else if (Server::findOperator(vec[0], arg) == -1 && sign == '+') {
		Server::_channels[index].setOperator(Server::retFd(arg), arg);
		// sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE " + channel + " +o " + arg + "\r\n");
		sendClients(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE "
			+ channel + " +o " + arg + "\r\n", channel.substr(1));
	}
	else if (Server::findOperator(vec[0], arg) != -1 && sign == '-') {
		Server::_channels[index].eraseOperator(Server::retFd(arg));
		// sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE " + channel + " -o " + arg + "\r\n");
		sendClients(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE "
			+ channel + " -o " + arg + "\r\n", channel.substr(1));
	}
}

void Client::checkKeyFlag(char sign, int index, std::string channel, std::string arg){
	if (sign == '+' && !Server::_channels[index]._channelMode._key) {
		Server::_channels[index]._channelMode._key = true;
		Server::_channels[index].setKey(arg);
		sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE " + channel + " +k " + arg + "\r\n");
	}
	else if (sign == '-' && Server::_channels[index]._channelMode._key
		&& Server::_channels[index].getKey() == arg) {
		Server::_channels[index]._channelMode._key = false;
		Server::_channels[index].setKey("");
		sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE " + channel + " -k " + arg + "\r\n");
	}
	else if ((sign == '+' && Server::_channels[index]._channelMode._key)
		|| (sign == '-' && !Server::_channels[index]._channelMode._key)
		|| (Server::_channels[index].getKey() != arg))
		sendTo(":" + Server::_hostname + " 467 " + this->_nickName + " " + channel + " :Channel key already set\r\n");
}

bool isNbr(std::string str)
{
	for (size_t j = 0; j < str.length(); j++)
	{
		if (!isdigit(str[j]))
			return true;
	}
	return false;
}

void Client::Mode() {
	if (checkError())
		return ;
	if (vec.size() == 1)
	{
		checkFlag(vec[0]);
		return;
	}
	std::string str = "#" + vec[0];
	char sign = '+';
	int index = Server::retChannel(vec[0]);
	for (size_t i = 1; i < vec.size(); i++)
	{
		for (size_t j = 0; j < vec[i].length(); j++)
		{
			if (vec[i][j] == '-')
				sign = '-';
			else if (vec[i][j] == '+')
				sign = '+';
			else {
				switch (vec[i][j])
				{
				case 't':
					checkTopic(sign, index, str);
					break;
				case 'i':
					checkInvite(sign, index, str);
					break;
				case  'o':
					if (i + 1 < vec.size()) {
						checkOperatorFlag(sign, index, str, vec[i + 1]);
						vec.erase(vec.begin() + i + 1);
					}
					break;
				case  'l':
					if (i + 1 >= vec.size() && sign == '+')
						sendTo(":" + Server::_hostname + " 461 " + this->_nickName + " MODE " + " +l " + ":Not enough parameters\r\n");
					if (i + 1 < vec.size() && sign == '+') {
						if (!isNbr(vec[i + 1]) && !Server::_channels[index]._channelMode._limit && atoi(vec[i + 1].c_str()) > 0) {
							Server::_channels[index].setlimitMbr(atoi(vec[i + 1].c_str()));
							Server::_channels[index]._channelMode._limit = true;
							sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " MODE " + str + " +l " + vec[i + 1] + "\r\n");
						}
						vec.erase(vec.begin() + i + 1);
					}
					if (sign == '-' && Server::_channels[index]._channelMode._limit) {
						Server::_channels[index].setlimitMbr(0);
						Server::_channels[index]._channelMode._limit = false;
						sendTo(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp +  " MODE " + str + " -l " + "\r\n");
					}
					break;
				case  'k':
					if (i + 1 < vec.size()) {
						checkKeyFlag(sign, index, str, vec[i + 1]);
						vec.erase(vec.begin() + i + 1);
					}
					else
						sendTo(":" + Server::_hostname + " 461 " + this->_nickName + " MODE " + sign + "k " + ":Not enough parameters\r\n");
					break;
				default:
					sendTo(ERR_UNKNOWNMODE(this->_nickName, Server::_hostname, str, vec[i][j]));
					break;
				}
			}
		}
	}
}