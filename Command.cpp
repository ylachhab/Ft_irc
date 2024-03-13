#include "Client.hpp"

void Client::sendTo(std::string msg) {
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void sendClient(std::string msg, int fd) {
	send(fd, msg.c_str(), msg.length(), 0);
}

void Client::Kick() {
	if (!this->_authenticated || !this->_registred) {
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
		return ;
	}
	if (vec.size() < 2) {
		sendTo(ERR_NEEDMOREPARAMS(this->_nickName, Server::_hostname));
		return ;
	}
	std::string str = vec[0];
	if (str[0] == '#' && str.length() > 2)
		vec[0] = vec[0].substr(1);
	else {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		// sendTo(this->_nickName + " " + str + " :No such channel" + "\r\n");
		return ;
	}
	if (!Server::findChannel(vec[0])) {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		// sendTo(this->_nickName + " " + str + " :No such channel\r\n");
		return ;
	}
	if (Server::findOperator(vec[0], this->_nickName) != this->_fd) {
		sendTo(ERR_NOTOP(Server::_hostname, str));
		// sendTo(this->_nickName + " " + str + " :You're not channel operator\r\n");
		return ;
	}
	if (Server::existeNick(vec[1]) == false) {
		sendTo(ERR_NOSUCHNICK(Server::_hostname, str, vec[1]));
		// sendTo(this->_nickName  + " " + vec[1] + " :No such nick\r\n");
		return;
	}
	if (!Server::isMember(vec[0], vec[1])) {
		sendTo(ERR_USERNOTINCHANNEL(Server::_hostname, vec[1]));
		// sendTo(this->_nickName + " " + vec[1]  + " "+ str +  ":They aren't on that channel\r\n");
		return ;
	}
	Server::eraseMember(vec[0], vec[1]);
	if (vec.size() > 3) {
		sendTo(Server::_hostname + " KICK " + str + " " + vec[1] + " :" + vec[vec.size() - 1] + "\r\n");
		sendClient(Server::_hostname + " KICK " + str + " " + vec[1] + " :" + vec[vec.size() - 1] + "\r\n",
				Server::retFd(vec[1]));
	}
	else {
		sendTo(Server::_hostname + " KICK " + str + " " + vec[1] + " :" + vec[1] + "\r\n");
		sendClient(Server::_hostname + " KICK " + str + " " + vec[1] + " :" + vec[1] + "\r\n",
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
		sendTo(ERR_NEEDMOREPARAMS(this->_nickName, Server::_hostname));
		// sendTo(this->_nickName + " INVITE :Not enough parameters" + "\r\n");
		return;
	}
	if (!Server::existeNick(vec[0]))
	{
		sendTo(ERR_NOSUCHNICK(Server::_hostname, vec[1], vec[0]));
		// sendTo(this->_nickName + " " + vec[0] + " :No such nick" + "\r\n");
		return ;
	}
	std::string str = vec[1];
	std::cout << str[1] << std::endl;
	if (str[0] == '#' && str.length() > 2)
		vec[1] = vec[1].substr(1);
	else {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		// sendTo(this->_nickName + " " + vec[1] + " :No such channel**" + "\r\n");
		return ;
	}
	if (!Server::findChannel(vec[1])) {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		// sendTo(this->_nickName + " " + str + " :No such channel++" + "\r\n");
		return ;
	}
	if (Server::isMember(vec[1], vec[0]))
	{
		sendTo(ERR_USERONCHANNEL(Server::_hostname, str, ));
		// sendTo(this->_nickName + " " + vec[0] + " " + vec[1] + " :is already on channel" + "\r\n");
		return ;
	}
	if (Server::findOperator(vec[1], this->_nickName) != this->_fd) {
		sendTo(ERR_NOTOP(Server::_hostname, str));
		// sendTo(this->_nickName + " " + vec[1] + " :You're not channel operator" + "\r\n");
		return ;
	}
	channelInvite.push_back(vec[1]);
	sendTo(Server::_hostname + " INVITE " + this->_nickName + " " + vec[1]+ "\r\n");
	sendClient(Server::_hostname + " INVITE " + this->_nickName + " " + vec[0] + " " + vec[1]+ "\r\n", Server::retFd(vec[0]));
}

void Client::Topic() {
	if (!this->_authenticated || !this->_registred)
	{
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
		return ;
	}
	std::string str = vec[0];
	if (str[0] == '#' && str.length() > 2)
		vec[0] = vec[0].substr(1);
	else {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		return;
	}
	if (!Server::findChannel(vec[0])) {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		return;
	}
	int index = Server::retChannel(vec[0]);
	if (Server::findOperator(vec[0], this->_nickName) != this->_fd && Server::_channels[index]._channelMode._topic) {
		sendTo(ERR_NOTOP(Server::_hostname, str));
		return;
	}
	if (vec.size() == 1) {
		if (!Server::_channels[index].getChannelTopic().empty())
			sendTo(":" + this->_nickName + " TOPIC " + str + " :" + Server::_channels[index].getChannelTopic() + "\r\n");
		else
			sendTo(":" + Server::_hostname + " 331 " + this->_nickName + " " + str + " :No topic is set.\r\n");
		return ;
	}
	Server::_channels[index].setChannelTopic(vec[vec.size() - 1]);
	sendTo(":" + this->_nickName + " TOPIC " + str + " :" + Server::_channels[index].getChannelTopic() + "\r\n");
}
