#include "Client.hpp"
#include "Server.hpp"

Client::Client()
{
	this->_authenticated = false;
	this->_registred = false;
	this->_pass = false;
	this->_nick = false;
	this->_user = false;
	this->_nickName = "*";
	this->sendMsg = false;
}

Client::Client(const Client& obj)
{
	*this = obj;
}

Client& Client::operator=(const Client& obj)
{
	if (this != &obj)
	{
		this->buffer = obj.buffer;
		this->vec = obj.vec;

		this->_authenticated = obj._authenticated;
		this->_registred = obj._registred;
		this->_userName = obj._userName;
		this->_realName = obj._realName;
		this->_fd = obj._fd;
		this->_nickName = obj._nickName;
		this->_password = obj._password;
		this->_port = obj._port;
		this->_pass = obj._pass;
		this->_user = obj._user;
		this->_nick = obj._nick;
		std::memcpy(this->clientIp, obj.clientIp, INET_ADDRSTRLEN);
	}
	return *this;
}

Client::~Client()
{
}

//---------------Getter-----------------

std::string Client::getPassword() const
{
	return this->_password;
}

std::string Client::getUserName() const
{
	return this->_userName;
}

std::string Client::getNickName() const
{
	return this->_nickName;
}

std::string Client::getRealName() const
{
	return this->_realName;
}


//---------------Setter-----------------

void Client::setPassword(const std::string& pass)
{
	this->_password = pass;
}

void Client::setNickName(const std::string& nickname)
{
	this->_nickName = nickname;
}

void Client::setClientIp(const std::string ip)
{
	std::memcpy(this->clientIp, ip.c_str(), INET_ADDRSTRLEN);
}

void Client::setFd(int fd)
{
	this->_fd = fd;
}
int Client::getFd() const
{
	return this->_fd;
}

/*************Parce && execute Commands*****************/

std::string Client::to_Upper(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = toupper(str[i]);
	return str;
}

void Client::parceCommand() {
	size_t found = 0;
	while ((found = buffer.find("\n", found)) != std::string::npos)
	{
		if (found == 0 || buffer[found - 1] != '\r')
		{
			buffer.insert(found, "\r");
			found += 1;
		}
		found += 1;
	}
	std::string str;
	std::string tmp = buffer;
	while (1)
	{
		std::string cmd;
		size_t start = tmp.find_first_not_of(" \t");
		if (start != std::string::npos)
			tmp = tmp.substr(start);
		size_t end = tmp.find_first_of("\r");
		if (end != std::string::npos)
		{
			str = tmp.substr(0, end);
			tmp = tmp.substr(end + 2, tmp.length() - (end + 2));
		}
		size_t index = str.find_first_of(" \t");
		if (index != std::string::npos)
		{
			cmd = str.substr(0, index);
			str = str.substr(index);
		}
		else
		{
			cmd = str;
			str = "";
			if (cmd.empty())
				break;
		}
		index = str.find_first_not_of(" \t");
		if (index != std::string::npos)
		{
			if (str[index] == ':')
				vec.push_back(str.substr(index + 1));
			else
			{
				str = str.substr(index);
				std::string strTmp = str;
				char* split_str = std::strtok(const_cast<char*>(str.c_str()), " \t");
				while(split_str != NULL)
				{
					vec.push_back(split_str);
					split_str = std::strtok(NULL, " \t");
					strTmp = strTmp.substr(vec.back().size());
					size_t j = strTmp.find_first_not_of(" \t");
					if (j != std::string::npos)
						strTmp = strTmp.substr(j);
					if (split_str && split_str[0] == ':')
					{
						vec.push_back(strTmp.substr(1));
						break;
					}
				}
			}
		}
		if (to_Upper(cmd) == "PASS")
			executePass();
		else if (to_Upper(cmd) == "NICK")
			executeNick();
		else if (to_Upper(cmd) == "USER")
			executeUser();
		else if (to_Upper(cmd) == "JOIN")
			executeJoin();
		else if (to_Upper(cmd) == "KICK")
			Kick();
		else if (to_Upper(cmd) == "INVITE")
			Invite();
		else if (to_Upper(cmd) == "MODE")
			Mode();
		else if (to_Upper(cmd) == "TOPIC")
			Topic();
		else if (to_Upper(cmd) == "PRIVMSG")
			executePrivMsg();
		else if (to_Upper(cmd) == "NOTICE")
			executeNotice();
		else if (to_Upper(cmd) == "QUIT")
			executeQuit();
		else if (to_Upper(cmd) == "BOT")
			Bot();
		else if (to_Upper(cmd) == "PONG" || cmd == Server::_hostname) {
			vec.clear();
			continue;
		}
		else
			sendTo(ERR_UNKNOWNCOMMAND(this->_nickName, Server::_hostname, cmd));
		if (tmp.empty())
		{
			vec.clear();
			break ;
		}
		vec.clear();
	}
}

void Client::RecvClient(pollfd& pfd, bool &flag) {
	error = false;
	char buf[513];
	std::memset(buf, 0, sizeof buf);
	int nbyte = recv(pfd.fd, buf, sizeof buf - 1, 0);
	buffer.append(buf);
	if (nbyte <= 0) {
		if (nbyte == 0)
			std::cout << "pollserver: socket " << this->_fd <<" hung up\n";
		else
			std::cout << "Error in recv\n";
		close(pfd.fd);
		error = true;
	}
	else {
		if (std::strlen(buf) && buf[std::strlen(buf) - 1] != '\n')
		{
			flag = true;
			return ;
		}
		parceCommand();
	}
	buffer.clear();
}
