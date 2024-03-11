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

//---------Setter--------

void Channel::setChannelName(std::string name) {
	this->_channelName = name;
}

void Channel::setOperator(int oper, std::string nickname) {
	this->_operator[oper] = nickname;
}

void Channel::setChannelPwd(std::string passw) {
	this->ChannelPwd = passw;
}

void Channel::setlimitMbr(int nbr) {
	this->limitMbr = nbr;
}

//---------Getter--------

std::string Channel::getChannelName() {
	return (this->_channelName);
}

std::string Channel::getChannelPassword() const{
	return this->ChannelPwd;
}

std::vector<Client> &Channel::getChannel()
{
	return this->_channel;
}

std::string Channel::getChannelPwd() const {
	return this->ChannelPwd;
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
	if (!_inviteOnly && !_topic && !_key && !_opera && !_limit)
		return true;
	return false;
}

bool ChannelMode::allOn(){
	if (_inviteOnly && _topic && _key && _opera && _limit)
		return true;
	return false;
}