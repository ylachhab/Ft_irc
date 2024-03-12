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
		return ;
	}
	if (vec.size() < 2) {
		sendTo(this->_nickName + " Kick :Not enough parameters" + "\r\n");
		return ;
	}
	std::string str = vec[0];
	if (str[0] == '#' && str.length() > 2)
		vec[0] = vec[0].substr(1);
	else {
		sendTo(this->_nickName + " " + str + " :No such channel++" + "\r\n");
		return ;
	}
	if (!Server::findChannel(vec[0])) {
		sendTo(this->_nickName + " " + str + " :No such channel\r\n");
		return ;
	}
	if (Server::findOperator(vec[0], this->_nickName) != this->_fd) {
		sendTo(this->_nickName + " " + str + " :You're not channel operator\r\n");
		return ;
	}
	if (Server::existeNick(vec[1]) == false) {
		sendTo(this->_nickName  + " " + vec[1] + " :No such nick\r\n");
		return;
	}
	if (!Server::isMember(vec[0], vec[1])) {
		sendTo(this->_nickName + " " + vec[1]  + " "+ str +  ":They aren't on that channel\r\n");
		return ;
	}
	Server::eraseMember(vec[0], vec[1]);
	if (vec.size() > 3) {
		sendTo("KICK " + str + " " + vec[1] + " :" + vec[vec.size() - 1] + "\r\n");
		sendClient("KICK " + str + " " + vec[1] + " :" + vec[vec.size() - 1] + "\r\n",
				Server::retFd(vec[1]));
	}
	else {
		sendTo("KICK " + str + " " + vec[1] + " :" + vec[1] + "\r\n");
		sendClient("KICK " + str + " " + vec[1] + " :" + vec[1] + "\r\n",
				Server::retFd(vec[1]));
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
		return ;
	}
	if (vec.size() < 2)
	{
		sendTo(this->_nickName + " INVITE :Not enough parameters" + "\r\n");
		return;
	}
	if (!Server::existeNick(vec[0]))
	{
		sendTo(this->_nickName + " " + vec[0] + " :No such nick" + "\r\n");
		return ;
	}
	std::string str = vec[1];
	std::cout << str[1] << std::endl;
	if (str[0] == '#' && str.length() > 2)
		vec[1] = vec[1].substr(1);
	else {
		sendTo(this->_nickName + " " + vec[1] + " :No such channel**" + "\r\n");
		return ;
	}
	if (!Server::findChannel(vec[1])) {
		sendTo(this->_nickName + " " + str + " :No such channel++" + "\r\n");
		return ;
	}
	if (Server::isMember(vec[1], vec[0]))
	{
		sendTo(this->_nickName + " " + vec[0] + " " + vec[1] + " :is already on channel" + "\r\n");
		return ;
	}
	if (Server::findOperator(vec[1], this->_nickName) != this->_fd) {
		sendTo(this->_nickName + " " + vec[1] + " :You're not channel operator" + "\r\n");
		return ;
	}
	channelInvite.push_back(vec[1]);
	sendTo("INVITE " + this->_nickName + " " + vec[1]+ "\r\n");
	sendClient("INVITE " + this->_nickName + " " + vec[0] + " " + vec[1]+ "\r\n", Server::retFd(vec[0]));
}
