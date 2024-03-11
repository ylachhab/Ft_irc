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
	this->_maxChannel = 0;
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
		// this->_hostName = obj._hostName;
		this->_nickName = obj._nickName;
		this->_password = obj._password;
		this->_port = obj._port;
		this->_pass = obj._pass;
		this->_user = obj._user;
		this->_nick = obj._nick;
	}
	return *this;
}

Client::~Client()
{
}

/*************Getters & Setters*****************/
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

void Client::setPassword(const std::string& pass)
{
	this->_password = pass;
}

void Client::setFd(int fd)
{
	this->_fd = fd;
}
int Client::getFd() const
{
	return this->_fd;
}

//------------------------------------------

void Client::parceCommand() {
	size_t found = 0;
	while ((found = buffer.find("\n", found)) != std::string::npos)
	{
		if (buffer[found - 1] != '\r')
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
			std::cout << "Error this is not command" << std::endl;
			if (tmp.empty())
				break;
			continue;
		}
		index = str.find_first_not_of(" \t");
		if (index != std::string::npos)
		{
			if (str[index] == ':')
				vec.push_back(str.substr(index + 1));
			else
			{
				str = str.substr(index);
				char* split_str = std::strtok(const_cast<char*>(str.c_str()), " \t");
				while(split_str != NULL)
				{
					vec.push_back(split_str);
					split_str = std::strtok(NULL, " \t");
				}	
			}
		}
		else
		{
			std::cout << "Error this is not command++" << std::endl;
			if (tmp.empty())
				break;
			continue;
		}
		// for (size_t i = 0; i < vec.size(); i++)
		// {
		// 	std::cout << vec[i] << " ";
		// }
		// std::cout << std::endl;
		if (cmd == "PASS" ||  cmd == "pass")
			executePass(vec);
		else if (cmd == "NICK" || cmd == "nick")
			executeNick(vec);
		else if (cmd == "USER" || cmd == "user")
			executeUser(vec);
		else if (cmd == "JOIN" || cmd == "join")
			executeJoin(vec);
		else if (cmd == "KICK" || cmd == "kick")
			Kick();
		else if (cmd == "INVITE" || cmd == "invite")
			Invite();
		else if (cmd == "MODE" || cmd == "mode")
			Mode();
		if (tmp.empty())
		{
			vec.clear();
			break ;
		}
		vec.clear();
	}
}

void Client::RecvClient(pollfd& pfd, int sockfd, bool &flag) {
	error = false;
	char buf[512];
	std::memset(&buf, 0, sizeof buf);
	int nbyte = recv(pfd.fd, buf, sizeof buf, 0);
	buffer.append(buf);
	int clientFd = pfd.fd; 
	if (nbyte <= 0) {
		if (nbyte == 0)
			std::cout << "pollserver: socket " << clientFd <<" hung up\n";
		else {
			std::cout << "Error in recv\n";
			std::exit(1);
		}
		close(pfd.fd);
		error = true;
	}
	else {
		if (buf[std::strlen(buf) - 1] != '\n')
		{
			flag = true;
			return ;
		}
		// std::cout << buffer << std::endl;
		parceCommand();
		int destFd = pfd.fd;
		if (destFd != sockfd && destFd != clientFd)
		{
			if (send(destFd, buf, nbyte, 0) == -1)
			{
				std::cout << "Error in send" << std::endl;
			}
		}
	}
	buffer.clear();
}
