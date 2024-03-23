#include "Client.hpp"

void Client::sendTo(std::string msg) {
	send(this->_fd, msg.c_str(), msg.length(), 0);
}

void Client::sendClients(std::string msg, std::string channel) {
	for (size_t i = 0; i < Server::cObjs.size(); i++)
	{
		if (Server::isMember(channel, Server::cObjs[i].getNickName()))
			send(Server::cObjs[i].getFd(), msg.c_str(), msg.length(), 0);
		if(channel == "" && Server::cObjs[i].getNickName() != this->_nickName && Server::cObjs[i]._registred)
		{
			std::string toSend =":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " PRIVMSG " 
				+ Server::cObjs[i].getNickName() + " :" + vec[vec.size() - 1] + "\r\n";
			send(Server::cObjs[i].getFd(), toSend.c_str(), toSend.length(), 0);
		}
	}
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
	if (str[0] == '#' && str.length() > 1)
		vec[0] = vec[0].substr(1);
	else {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		return ;
	}
	if (!Server::findChannel(vec[0])) {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		return ;
	}
	if (Server::findOperator(vec[0], this->_nickName) != this->_fd) {
		sendTo(ERR_NOTOP(Server::_hostname, str));
		return ;
	}
	if (Server::existeNick(vec[1]) == false) {
		sendTo(ERR_NOSUCHNICK(Server::_hostname, str, vec[1]));
		return;
	}
	if (!Server::isMember(vec[0], vec[1])) {
		sendTo(ERR_USERNOTINCHANNEL(Server::_hostname, vec[1]));
		return ;
	}
	Server::eraseMember(vec[0], vec[1]);
	int Cfd = Server::retFd(vec[1]);
	Server::_channels[Server::retChannel(vec[0])].getOperator().erase(Cfd);
	if (vec.size() > 3) {
		std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " KICK "
			+ str + " " + vec[1] + " :" + vec[vec.size() - 1] + "\r\n";
		sendClients(msg, vec[0]);
		send(Cfd, msg.c_str(), msg.length(), 0);
	}
	else {
		std::string msg = ":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " KICK "
			+ str + " " + vec[1] + " :" + vec[1] + "\r\n";
		sendClients(msg, vec[0]);
		send(Cfd, msg.c_str(), msg.length(), 0);
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
		return;
	}
	if (!Server::existeNick(vec[0]))
	{
		sendTo(ERR_NOSUCHNICK(Server::_hostname, vec[1], vec[0]));
		return ;
	}
	std::string str = vec[1];
	if (str[0] == '#' && str.length() > 1)
		vec[1] = vec[1].substr(1);
	else {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		return ;
	}
	if (!Server::findChannel(vec[1])) {
		sendTo(ERR_NOSUCHCHANNEL(Server::_hostname, str, this->_nickName));
		return ;
	}
	if (Server::isMember(vec[1], vec[0]))
	{
		sendTo(ERR_USERONCHANNEL(Server::_hostname, str, ));
		return ;
	}
	if (Server::findOperator(vec[1], this->_nickName) != this->_fd) {
		sendTo(ERR_NOTOP(Server::_hostname, str));
		return ;
	}
	if (!isInvited(vec[1]))
	{
		int i = Server::retClient(vec[0]);
		Server::cObjs[i].channelInvite.push_back(vec[1]);
	}
	sendTo(":" + Server::_hostname + " 341 " + this->_nickName + " " + vec[0] + " " + vec[1]+ "\r\n");
	sendClient(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " INVITE "
		+ vec[0] + " " + str + "\r\n", Server::retFd(vec[0]));
}

void Client::Topic() {
	if (!this->_authenticated || !this->_registred)
	{
		sendTo(ERR_NOTREGISTERED(this->_nickName, Server::_hostname));
		return ;
	}
	std::string str = vec[0];
	if (str[0] == '#' && str.length() > 1)
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
	if (Server::findOperator(vec[0], this->_nickName) != this->_fd && Server::_channels[index]._channelMode._topic && vec.size() > 1) {
		sendTo(ERR_NOTOP(Server::_hostname, str));
		return;
	}
	if (vec.size() == 1) {
		if (!Server::_channels[index].getChannelTopic().empty())
			sendTo(":" + Server::_hostname + " 332 " + this->_nickName + " " + str + " :" + Server::_channels[index].getChannelTopic() + "\r\n");
		else
			sendTo(":" + Server::_hostname + " 331 " + this->_nickName + " " + str + " :No topic is set.\r\n");
		return ;
	}
	Server::_channels[index].setChannelTopic(vec[vec.size() - 1]);
	sendClients(":" + this->_nickName + "!~" + this->_userName + "@" + this->clientIp + " TOPIC " + str + " :"
		+ Server::_channels[index].getChannelTopic() + "\r\n", vec[0]);
}
