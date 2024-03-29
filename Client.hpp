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
		std::string	_password;
		std::string _port;
		bool		_authenticated;
		bool		_registred;
		bool		_pass;
		bool		_nick;
		bool		_user;
		int			_fd;
		char		clientIp[INET_ADDRSTRLEN];
	public:
		bool error;
		//Orthodox canonical class form
		Client();
		Client(const Client& obj);
		Client& operator=(const Client& obj);
		~Client();

		//Parce and execute Command
		void RecvClient(pollfd& pfd, bool &flag);
		void parceCommand();
		std::string to_Upper(std::string str);

		//----------Setter---------
		void setFd(int fd);
		void setPassword(const std::string& pass);
		void setClientIp(std::string ip);
		void setNickName(const std::string& nickname);
		// //-------------Getter--------------
		std::string getPassword() const;
		std::string getNickName() const;
		std::string getUserName() const;
		std::string getRealName() const;
		int getFd() const;
		//---------------Commands Utils---------------
		void sendClients(std::string msg, std::string channel);
		void sendToAll(std::string msg, std::string channel);
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
		void addNewChannel(std::string channelName);
		void addToExistChannel(int index, std::string channelName);
		std::vector<std::pair<std::string, std::string> > splitChannels(std::string& channels_Name);
		std::string removeExtraChar(const std::string& input, char del);
		int existChannel(std::string channelName);
		void nicknameSet(bool flag);
		void isRegesterd();
		void executeBotClient(Client &client);
		void executeBotChannel();
		void executeBot();
		int retInviteChan(std::string channel);
		// void addInvitedClient(int index, std::string channelName);
		//----------------Command---------------
		void executePass();
		void executeNick();
		void executeUser();
		void executeJoin();
		void executePrivMsg();
		void executeNotice();
		void executeQuit();
		void Bot();
		void Topic();
		void Mode();
		void Kick();
		void Invite();
};

#endif