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
#include "Server.hpp"
#include "Channel.hpp"


class Client
{
	private:
		std::string	buffer;
		std::vector <std::string > vec;
		std::string	_nickName;
		std::string	_userName;
		std::string	_hostName;
		bool		_authenticated;
		bool		_registed;
		std::string port;
		std::string passW;
		int			_fd;
	public:
		bool error;
		Client();
		Client(const Client& obj);
		Client& operator=(const Client& obj);
		~Client();
		void RecvClient(pollfd& pfd, int sockfd, bool &flag);
		void parceCommand();
		//----------Setter---------
		void setPort(std::string& port);
		void setPassw(std::string& passW);
		void setFd(int& fd);
		// void setNickName(std::string nickName);
		// void setUserName(std::string userName);
		// void setAuthenticated(bool authenticated);
		// void setRegisted(bool registed);
		// //-------------Getter--------------
		std::string getNickName() const;
		std::string getUserName() const;
		bool getAuthenticated() const;
		bool getRegisted() const;

		//----------------Command---------------
		void Kick();
		void Invite();
};

// void Topic();
// void Mode();

#endif