#include "Channel.hpp"

Channel::Channel()
{
	this->channelTopic = "";
}

Channel::Channel(const Channel& obj) {
	*this = obj;
}

Channel& Channel::operator=(const Channel& obj) {
	if (this != &obj) {
		this->_channelName = obj._channelName;
		this->_channel = obj._channel;
		this->_operator = obj._operator;
		this->channelTopic = obj.channelTopic;
		this->key = obj.key;
		this->limitMbr = obj.limitMbr;
	}
	return *this;
}

Channel::~Channel() {

}

//---------Setter--------

void Channel::setChannelName(std::string name) {
	this->_channelName = name;
}

void Channel::setOperator(int oper, std::string nickname) {
	this->_operator[oper] = nickname;
}

void Channel::setlimitMbr(int nbr) {
	this->limitMbr = nbr;
}

void Channel::setKey(std::string key) {
	this->key = key;
}

void Channel::setChannelTopic(std::string topic) {
	this->channelTopic = topic;
}

//---------Getter--------

std::string Channel::getChannelName() {
	return (this->_channelName);
}

std::vector<Client> &Channel::getChannel()
{
	return this->_channel;
}


int Channel::getlimitMbr() const {
	return this->limitMbr;
}

std::string Channel::getChannelName() const {
	return _channelName;
}

std::map<int, std::string> Channel::getOperator() const {
	return _operator;
}

std::string Channel::getKey() const {
	return this->key;
}

std::string Channel::getChannelTopic() const {
	return this->channelTopic;
}

//--------------------------------------

bool Channel::isAMember(std::string nick)
{
	for (size_t i = 0; i < _channel.size(); i++)
	{
		if (_channel[i].getNickName() == nick)
			return true;
	}
	return false;
}

void Channel::eraseMember(std::string nick) {
	for (size_t i = 0; i < _channel.size(); i++)
	{
		if (_channel[i].getNickName() == nick)
			_channel.erase(_channel.begin() + i);
	}
}

void Channel::eraseOperator(int fd) {
	this->_operator.erase(fd);
}

int Channel::isOperator(std::string nickname) {
	for (std::map<int, std::string>::iterator it = _operator.begin(); it != _operator.end(); ++it)
	{
		if (it->second == nickname)
			return it->first;
	}
	return -1;
}

ChannelMode::ChannelMode(){
	_inviteOnly = false;
	_topic = false;
	_key = false;
	_opera = false;
	_limit = false;
};

bool ChannelMode::allOff(){
	if (!_inviteOnly && !_topic && !_key && !_limit)
		return true;
	return false;
}

bool ChannelMode::allOn(){
	if (_inviteOnly && _topic && _key && _limit)
		return true;
	return false;
}

bool ChannelMode::IKLoff(){
	if (!_inviteOnly && !_key && !_limit)
		return true;
	return false;
}