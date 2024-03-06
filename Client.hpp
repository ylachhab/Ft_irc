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
		std::vector	<std::string > vec;
		std::string	_userName;
		std::string	_realName;
		// std::string	_hostName;
		std::string	_nickName;
		std::string	_password;
		std::string _port;
		bool		_authenticated;
		bool		_registred;
		bool		_pass;
		bool		_nick;
		bool		_user;
		bool		_fd;
	public:
		bool error;
		//Orthodox canonical class form
		Client();
		Client(const Client& obj);
		Client& operator=(const Client& obj);
		~Client();

		//Getters and setters
		const std::string& getPassword() const;
		const std::string& getUserName() const;
		void setPassword(const std::string& pass);
		void setFd(int fd);
		//Parce and execute Command
		void RecvClient(pollfd& pfd, int sockfd, bool &flag);
		void parceCommand();

		//Commands
		void executePass(std::vector<std::string> &vec);
		void executeNick(std::vector<std::string> &vec);
		void executeUser(std::vector<std::string> &vec);
};

#endif