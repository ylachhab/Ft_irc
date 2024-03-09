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

class Client
{
	private:
		std::string	buffer;
		std::vector <std::string > vec;
		std::vector <std::string > channelInvite;
		std::string	_nickName;
		std::string	_userName;
		std::string	_realName;
		// std::string	_hostName;
		std::string	_password;
		std::string _port;
		bool		_authenticated;
		bool		_registred;
		bool		_pass;
		bool		_nick;
		bool		_user;
		int			_fd;
		int			_maxChannel;
	public:
		bool error;
		//Orthodox canonical class form
		Client();
		Client(const Client& obj);
		Client& operator=(const Client& obj);
		~Client();

		//Parce and execute Command
		void RecvClient(pollfd& pfd, int sockfd, bool &flag);
		void parceCommand();

		//----------Setter---------
		void setFd(int fd);
		void setPassword(const std::string& pass);
		// void setMaxChannel(int max_channels);
		// void setNickName(std::string nickName);
		// void setUserName(std::string userName);
		// void setAuthenticated(bool authenticated);
		// void setRegisted(bool registed);

		// //-------------Getter--------------
		std::string getPassword() const;
		// int	 getMaxChannel() const;
		std::string getNickName() const;
		std::string getUserName() const;
		// bool getAuthenticated() const;
		// bool getRegisted() const;
		int getFd() const;

		//----------------Command---------------
		void Kick();

		//---------------
		void sendTo(std::string msg);
		bool isInvited(std::string channel);
		void Invite();

		//---------------
		void Mode();

// void Topic();

		void sendRepance(const std::string& str);
		void addNewChannel(std::string channelName);
		//Commands
		void executePass(std::vector<std::string> &vec);
		void executeNick(std::vector<std::string> &vec);
		void executeUser(std::vector<std::string> &vec);
		void executeJoin(std::vector<std::string> &vec);
};

#endif