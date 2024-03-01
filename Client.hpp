#ifndef CLIENT
#define CLIENT

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

class Client
{
	private:
		std::string	buffer;
		std::vector <std::string > PassC;
		// std::string	_nickName;
		// std::string	_userName;
		// std::string	_hostName;
		// bool		_authenticated;
		// bool		_registed;
	public:
		bool error;
		Client();
		Client(const Client& obj);
		Client& operator=(const Client& obj);
		~Client();
		void RecvClient(pollfd& pfd, int sockfd, bool &flag);
		void parceCommand();
		//pass
		//nick
};

#endif