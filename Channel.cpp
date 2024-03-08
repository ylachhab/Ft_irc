#include "Channel.hpp"

Channel::Channel()
{
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

// void Channel::setChannel(Client &cl) {
// 	this->_channel.push_back(cl);
// }

std::vector<Client> &Channel::getChannel()
{
	return this->_channel;
}

void Channel::setOperator(int oper) {
	this->_operator = oper;
}

std::string Channel::getChannelName() const {
	return _channelName;
}

int Channel::getOperator() const {
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