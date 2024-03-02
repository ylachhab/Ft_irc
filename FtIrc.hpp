#ifndef FTIRC
#define FTIRC	

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

class FtIrc
{
	private:
		std::string port;
		std::string password;
		std::vector <pollfd > pfds;
	public:
		static std::vector <Client> cObjs;
		FtIrc();
		FtIrc(std::string port, std::string password);
		FtIrc(const FtIrc& obj);
		FtIrc& operator=(const FtIrc& obj);
		~FtIrc();
		int get_socket();
		void addToPfds(int newfd);
		void deletePfds(int i);
};

#endif