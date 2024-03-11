#include "Channel.hpp"

Channel::Channel()
{
	this->_passSet = false;
}

Channel::Channel(const Channel& obj) {
	*this = obj;
}

Channel& Channel::operator=(const Channel& obj) {
	if (this != &obj) {
		this->_channelName = obj._channelName;
		this->_channel = obj._channel;
		this->_operator = obj._operator;
	}
	return *this;
}

Channel::~Channel() {

}

void Channel::setChannelName(std::string name) {
	this->_channelName = name;
}

std::string Channel::getChannelName() {
	return (this->_channelName);
}

std::string Channel::getChannelPassword() const{
	return this->Channel_password;
}

// void Channel::setChannel(Client &cl) {
// 	this->_channel.push_back(cl);
// }

std::vector<Client> &Channel::getChannel()
{
	return this->_channel;
}

void Channel::setOperator(int oper, std::string nickname) {
	this->_operator[oper] = nickname;
}

std::string Channel::getChannelName() const {
	return _channelName;
}

std::map<int, std::string> Channel::getOperator() const {
	return _operator;
}

bool Channel::isAMember(std::string nickName) const
{
	for (size_t i = 0; i < _channel.size(); i++)
	{
		if (_channel[i].getNickName() == nickName)
			return true;
	}
	return false;
}

void Channel::eraseMember(std::string nickName) {
	// if (std::find(_channel.begin(), _channel.end(), obj) != _channel.end())
	// 	_channel.erase(std::find(_channel.begin(), _channel.end(), obj));
	for (size_t i = 0; i < _channel.size(); i++)
	{
		if (_channel[i].getNickName() == nickName)
			_channel.erase(_channel.begin() + i);
	}
}
