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
#include "Replies_msg.hpp"

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
		char		clientIp[INET_ADDRSTRLEN];
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
		void setClientIp(std::string ip);
		// void setMaxChannel(int max_channels);
		// void setNickName(std::string nickName);
		// void setUserName(std::string userName);
		// void setAuthenticated(bool authenticated);
		// void setRegisted(bool registed);

		// //-------------Getter--------------
		std::string getPassword() const;
		// void getClientIp(std::string ip);
		// int	 getMaxChannel() const;
		std::string getNickName() const;
		std::string getUserName() const;
		// bool getAuthenticated() const;
		// bool getRegisted() const;
		int getFd() const;

		//---------------Commands Utils---------------
		void sendClients(std::string msg, std::string channel);
		void sendTo(std::string msg);
		bool isInvited(std::string channel);
		//---------------
		bool checkError();
		void checkFlag(std::string channel);
		//---------------
		void checkTopic(char sign, int index, std::string channel);
		void checkInvite(char sign, int index, std::string channel);
		void checkOperatorFlag(char sign, int index, std::string channel, std::string arg);
		void checkKeyFlag(char sign, int index, std::string channel, std::string arg);
		//---------------
		void sendRepance(const std::string& str);
		void addNewChannel(std::string channelName);
		void addToExistChannel(int index, std::string channelName);
		std::vector<std::pair<std::string, std::string> > splitChannels(std::string& channels_Name);
		std::string removeExtraChar(const std::string& input, char del);
		void sendMsg(Client &client);
		//----------------Command---------------
		void executePass();
		void executeNick();
		void executeUser();
		void executeJoin();
		void executePrivMsg();
		void Topic();
		void Mode();
		void Kick();
		void Invite();
};

#endif