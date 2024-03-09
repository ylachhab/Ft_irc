#ifndef SERVER
#define SERVER	

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
#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Channel;
class Server
{
	private:
		std::string port;
		std::string password;
		std::vector <pollfd > pfds;
	public:
		static std::vector <Client > cObjs;
		static std::vector <Channel > _channels;
		Server();
		Server(std::string port, std::string password);
		Server(const Server& obj);
		Server& operator=(const Server& obj);
		~Server();
		int get_socket();
		void addToPfds(int newfd);
		void deletePfds(int i);
		static int findChannel(std::string channel);
		static bool isMember(std::string channel, std::string nickName);
		static bool existeNick( std::string nickName);
		static int retFd( std::string nickName);
		static void eraseMember(std::string channel, std::string nick);
};

#endif