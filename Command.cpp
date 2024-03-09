#include "Client.hpp"

void Client::sendTo(std::string msg) {
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void sendClient(std::string msg, int fd) {
	send(fd, msg.c_str(), msg.length(), 0);
}

void Client::Kick() {
	if (!this->_authenticated || !this->_registred) {
		sendTo(" :Register first.\r\n");
		// std::cout << " :Register first." << std::endl;
		return ;
	}
	if (vec.size() < 2) {
		sendTo(this->_nickName + " Kick :Not enough parameters" + "\r\n");
		// std::cout << "Kick :Not enough parameters" << std::endl;
		return ;
	}
	std::string str = vec[0];
	if (str[0] == '#' && str.length() > 2)
		vec[0].substr(1);
	else {
		sendTo(this->_nickName + str + " :No such channel" + "\r\n");
		// std::cout << str << " :No such channel" << std::endl;
		return ;
	}
	if (Server::findChannel(vec[0]) == -1) {
		sendTo(this->_nickName + str + " :No such channel\r\n");
		return ;
	}
	if (Server::findChannel(vec[0]) != this->_fd) {
		sendTo(this->_nickName + str + " :You're not channel operator\r\n");
		// std::cout << _fd << " :You're not channel operator" << std::endl;
		return ;
	}
	if (Server::existeNick(vec[1]) == false) {
		sendTo(this->_nickName + vec[1] + " :No such nick\r\n");
		return;
	}
	if (!Server::isMember(vec[0], vec[1])) {
		sendTo(this->_nickName + vec[1] + str +  ":They aren't on that channel\r\n");
		// std::cout << ":They aren't on that channel" << std::endl;
		return ;
	}
	Server::eraseMember(vec[0], vec[1]);
	if (vec.size() > 3) {
		sendTo("KICK " + str + " " + vec[1] + " :" + vec[vec.size() - 1] + "\r\n");
		sendClient("KICK " + str + " " + vec[1] + " :" + vec[vec.size() - 1] + "\r\n",
				Server::retFd(vec[1]));
		// std::cout << "KICK #" << str
		// 	<< vec[vec.size() - 1] << " :" << vec[vec.size() - 1] << std::endl;
	}
	else {
		sendTo("KICK " + str + " " + vec[1] + " :" + vec[1] + "\r\n");
		sendClient("KICK " + str + " " + vec[1] + " :" + vec[1] + "\r\n",
				Server::retFd(vec[1]));
		// std::cout << "KICK #" << Server::_channels[i].getChannelName()
		// 	<< vec[1] << " :" << vec[1] << std::endl;
	}
}

bool Client::isInvited(std::string channel) {
	for (size_t i = 0; i < channelInvite.size(); i++)
	{
		if (channelInvite[i] == channel)
			return true;
	}
	return  false;
}

void Client::Invite() {
	if (!this->_authenticated || !this->_registred)
	{
		sendTo(" :Register first.\r\n");
		// std::cout << " :Register first." << std::endl;
		return ;
	}
	if (vec.size() < 2)
	{
		sendTo(this->_nickName + " INVITE :Not enough parameters" + "\r\n");
		// std::cout << this->_nickName << " INVITE :Not enough parameters" << std::endl;
		return;
	}
	if (!Server::existeNick(vec[0]))
	{
		sendTo(this->_nickName + vec[0] + " :No such nick" + "\r\n");
		// std::cout << this->_nickName << vec[0] << " :No such nick" << std::endl;
		return ;
	}
	std::string str = vec[1];
	if (str[1] == '#' && str.length() > 2)
		vec[1].substr(1);
	else {
		sendTo(this->_nickName + vec[1] + " :No such channel" + "\r\n");
		// std::cout << this->_nickName << vec[1] << " :No such channel" << std::endl;
		return ;
	}
	if (Server::findChannel(vec[1]) == -1) {
		sendTo(this->_nickName + str + " :No such channel" + "\r\n");
		// std::cout << this->_nickName << str << " :No such channel" << std::endl;
		return ;
	}
	if (Server::isMember(vec[1], vec[0]))
	{
		sendTo(this->_nickName + vec[0] + vec[1] + " :is already on channel" + "\r\n");
		return ;
	}
	if (Server::findChannel(vec[1]) != this->_fd) {
		sendTo(this->_nickName + vec[1] + " :You're not channel operator" + "\r\n");
		// std::cout << this->_nickName << " :You're not channel operator" << std::endl;
		return ;
	}
	channelInvite.push_back(vec[1]);
	sendTo("INVITE " + this->_nickName + vec[1]+ "\r\n");
	sendClient(this->_nickName + vec[0] + vec[1]+ "\r\n", Server::retFd(vec[0]));
}


void Client::Mode() {
	if (!this->_authenticated || !this->_registred)
	{
		sendTo(" :Register first.\r\n");
		std::cout << " :Register first." << std::endl;
		return ;
	}
	if (vec.size() == 1)
	{
		sendTo(this->_nickName + " MODE :Not enough parameters" + "\r\n");
		// std::cout << this->_nickName << " MODE :Not enough parameters" << std::endl;
		return;
	}
	std::string str = vec[0];
	if (str[1] == '#' && str.length() > 2)
		vec[0].substr(1);
	else {
		sendTo(this->_nickName + vec[0] + " :No such channel" + "\r\n");
		// std::cout << this->_nickName << vec[0] << " :No such channel" << std::endl;
		return ;
	}
	if (Server::findChannel(vec[0]) == -1) {
		sendTo(this->_nickName + str + " :No such channel" + "\r\n");
		// std::cout << this->_nickName << str << " :No such channel" << std::endl;
		return ;
	}
	if (Server::findChannel(vec[0]) != this->_fd) {
		sendTo(this->_nickName + str + " :You're not channel operator" + "\r\n");
		// std::cout << this->_nickName << " :You're not channel operator" << std::endl;
		return ;
	}
}
