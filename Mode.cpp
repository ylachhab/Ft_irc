#include "Client.hpp"

void Client::checkFlag(std::string channel) {
	int i = Server::retChannel(channel);
	std::string sub;

	if (Server::_channels[i]._channelMode.allOff())
		sub = "t";
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
		sendTo(this->_nickName + " #" + channel + " +" + sub + " " + lim + " " + Server::_channels[i].getChannelPwd() + "\r\n");
	}
	else if (Server::_channels[i]._channelMode._limit)
	{
		std::stringstream ss(Server::_channels[i].getlimitMbr());
		std::string lim;
		ss >> lim;
		sendTo(this->_nickName + " #" + channel + " +" + sub + " " + lim + "\r\n");
	}
	else if (Server::_channels[i]._channelMode._key)
		sendTo(this->_nickName + " #" + channel + " +" + sub + " " + Server::_channels[i].getChannelPwd() + "\r\n");
	else
		sendTo(this->_nickName + " #" + channel + " +" + sub + "\r\n");
}

void Client::Mode() {
	if (!this->_authenticated || !this->_registred)
	{
		sendTo(" :Register first.\r\n");
		std::cout << " :Register first." << std::endl;
		return ;
	}
	// if (vec.size() == 1)
	// {
	// 	sendTo(this->_nickName + " MODE :Not enough parameters" + "\r\n");
	// 	return;
	// }
	std::string str = vec[0];
	if (str[0] == '#' && str.length() > 2)
		vec[0] = vec[0].substr(1);
	else {
		sendTo(this->_nickName + " " + vec[0] + " :No such channel" + "\r\n");
		return ;
	}
	if (!Server::findChannel(vec[0])) {
		sendTo(this->_nickName + " " + str + " :No such channel" + "\r\n");
		return ;
	}
	if (Server::findOperator(vec[0], this->_nickName) != this->_fd) {
		sendTo(this->_nickName + " " + str + " :You're not channel operator" + "\r\n");
		return ;
	}
	if (vec.size() == 1)
	{
		checkFlag(vec[0]);
		return ;
	}
}