#include "FtIrc.hpp"

FtIrc::FtIrc()
{
}

FtIrc::FtIrc(const FtIrc& obj)
{
	*this = obj;
}

FtIrc& FtIrc::operator=(const FtIrc& obj)
{
	(void)obj;
	return *this;
}

FtIrc::~FtIrc()
{
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

int FtIrc::get_socket() {
	int socfd;
	int yes = 1;
	int rv;

	struct addrinfo hints, *ai, *p;
	std::memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
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

void FtIrc::addToPfds(int newfd){
	struct pollfd tmp;
	tmp.fd = newfd;
	tmp.events = POLLIN;
	pfds.push_back(tmp);
}

void *getAddr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET)
		return &((struct sockaddr_in *)sa)->sin_addr;
	return &((struct sockaddr_in6 *) sa)->sin6_addr;
}

void FtIrc::deletePfds(int i){
	pfds.erase(pfds.begin() +i);
}

FtIrc::FtIrc(std::string port, std::string password) {
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
	char buf[512];
	std::memset(&buf, 0, sizeof buf);
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
						addToPfds(newFd);
						std::cout << "pollserver: new connection from " << inet_ntop(cAddr.ss_family, 
							getAddr((struct sockaddr*)&cAddr), remoteIP, INET6_ADDRSTRLEN)
							<< " on socket " << newFd << std::endl;
					}
				}
				else {
					int nbyte = recv(pfds[i].fd, buf, sizeof buf, 0);
					int clientFd = pfds[i].fd;
					if (nbyte <= 0) {
						if (nbyte == 0)
							std::cout << "pollserver: socket " << clientFd <<" hung up\n";
						else {
							std::cout << "Error in recv\n";
							std::exit(1);
						}
						close(pfds[i].fd);
						pfds.erase(pfds.begin() + i);
					}
					else {
						if (buf[std::strlen(buf) - 1] != '\n')
						{
							std::memset(&buf, 0, sizeof buf);
							continue;
						}
						std::cout << "To send: " << buf;
						for (size_t j = 0; j < pfds.size(); j++)
						{
							int destFd = pfds[j].fd;
							if (destFd != sockfd && destFd != clientFd)
							{
								if (send(destFd, buf, nbyte, 0) == -1)
								{
									std::cout << "Error in send" << std::endl;
								}
							}
						}
					}
				}
			}
		}
	}
}
