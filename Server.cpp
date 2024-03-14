#include "Server.hpp"

std::vector <Client > Server::cObjs;
std::vector <Channel > Server::_channels;
std::string Server::_hostname = "FT_IRC.1337.ma";

Server::Server()
{
}

Server::Server(const Server& obj)
{
	*this = obj;
}

Server& Server::operator=(const Server& obj)
{
	if (this != &obj)
	{
		this->port = obj.port;
		this->password = obj.password;
		this->pfds = obj.pfds;
	}
	return *this;
}

Server::~Server()
{
}

bool Server::existeNick( std::string nickName) {
	for (size_t i = 0; i < cObjs.size(); i++)
	{
		if (cObjs[i].getNickName() == nickName)
				return true;
	}
	return false;
}

int Server::retFd( std::string nickName) {
	for (size_t i = 0; i < cObjs.size(); i++)
	{
		if (cObjs[i].getNickName() == nickName)
				return cObjs[i].getFd();
	}
	return -1;
}

bool Server::isMember(std::string channel, std::string nickName) {
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getChannelName() == channel)
		{
			if (_channels[i].isAMember(nickName))
				return true;
			else
				return false;
		}
	}
	return false;
}

bool Server::findChannel(std::string channel) {
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getChannelName() == channel)
			return true;
	}
	return false;
}

int Server::retChannel(std::string channel) {
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getChannelName() == channel)
			return i;
	}
	return 0;
}

int Server::retClient(std::string Client) {
	for (size_t i = 0; i < cObjs.size(); i++)
	{
		if (cObjs[i].getNickName() == Client)
			return i;
	}
	return 0;
}

int Server::findOperator(std::string channel, std::string nick) {
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getChannelName() == channel)
		{
			if(_channels[i].isOperator(nick) == -1)
				return -1;
			return _channels[i].isOperator(nick);
		}
	}
	return -1;
}

void Server::eraseMember(std::string channel, std::string nick) {
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getChannelName() == channel)
			_channels[i].eraseMember(nick);
	}
}

int isNumber(std::string str)
{
	for (size_t j = 0; j < str.length(); j++)
	{
		if (!isdigit(str[j]))
			return (1);
	}
	return (0);
}

int Server::get_socket() {
	int socfd;
	int yes = 1;
	int rv;

	struct addrinfo hints, *ai, *p;
	std::memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(NULL, this->port.c_str(), &hints, &ai)))
	{
		std::cout << gai_strerror(rv) << std::endl;
		std::exit(1);
	}
	for (p = ai; p != NULL; p = p->ai_next)
	{
		socfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (socfd < 0)
			continue;
		if (setsockopt(socfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))== -1)
		{
			std::cout << "Error in setsockopt" << std::endl;
			std::exit(1);
		}
		if (bind(socfd, p->ai_addr, p->ai_addrlen) < 0)
		{
			close (socfd);
			continue;
		}
		break ;
	}
	freeaddrinfo(ai);
	if (p == NULL)
		return -1;
	if (listen(socfd, 10) == -1)
		return -1;
	return socfd;
}

void Server::addToPfds(int newfd){
	struct pollfd tmp;
	tmp.fd = newfd;
	tmp.events = POLLIN | POLLOUT;
	pfds.push_back(tmp);
}

void *getAddr(struct sockaddr *sa) {
	return &((struct sockaddr_in *)sa)->sin_addr;
}

void Server::deletePfds(int i){
	pfds.erase(pfds.begin() +i);
}

Server::Server(std::string port, std::string password) {
	if (isNumber(port) || port.empty())
	{
		std::cout << "Error in port" << std::endl;
		std::exit(1);
	}
	this->port = port;
	this->password = password;
	int sockfd = get_socket();
	if (sockfd == -1)
	{
		std::cout << "Error getting listening socket" << std::endl;
		std::exit(1);
	}
	addToPfds(sockfd);
	socklen_t addLen;
	struct sockaddr_storage cAddr;
	int newFd;
	char remoteIP[INET6_ADDRSTRLEN];
	bool flag;
	while (1) {
		int pcount = poll(pfds.data(), pfds.size(), -1);
		if (pcount == -1)
		{
			std::cout << "Error in poll" << std::endl;
			std::exit(1);
		}

		for (size_t i = 0; i < pfds.size(); i++)
		{
			if (pfds[i].revents & POLLIN)
			{
				if (pfds[i].fd == sockfd) {
					addLen = sizeof cAddr;
					newFd = accept(sockfd, (struct sockaddr *)&cAddr, &addLen);
					if (newFd == -1)
					{
						std::cout << "Error in accept" << std::endl;
						std::exit(1);
					}
					else {
						Client cObj;
						char *ipStr;
						struct sockaddr_in* AddrV4 = (struct sockaddr_in*)&cAddr;
						ipStr = inet_ntoa(AddrV4->sin_addr);
						addToPfds(newFd);
						cObj.setPassword(this->password);
						cObj.setClientIp(ipStr);
						std::cout << ipStr << std::endl;
						cObj.setFd(newFd);
						cObjs.push_back(cObj);
						std::cout << "pollserver: new connection from " << inet_ntop(cAddr.ss_family, 
							getAddr((struct sockaddr*)&cAddr), remoteIP, INET6_ADDRSTRLEN)
							<< " on socket " << newFd << std::endl;
					}
				}
				else {
					cObjs[i - 1].RecvClient(pfds[i], sockfd, flag);
					if (flag)
					{
						flag = false;
						continue;
					}
					if (cObjs[i - 1].error)
					{
						pfds.erase(pfds.begin() + i);
						cObjs.erase(cObjs.begin() + (i - 1));
					}
				}
			}
		}
	}
}