#ifndef CHANNEL
#define CHANNEL

#include <iostream>
#include <vector>
#include <algorithm>
#include "Client.hpp"
class Client;

class Channel {
	private :
		std::string _channelName;
		std::vector <Client > _channel;
		int _operator;
	public :
		Channel();
		Channel(const Channel& obj);
		Channel& operator=(const Channel& obj);
		~Channel();
		void setChannelName(std::string name);
		// void setChannel(Client cl);
		void setOperator(int oper);
		std::string getChannelName() const;
		int getOperator() const;
		bool isAMember(std::string nick);
		void eraseMember(std::string nick);
		// Client ret(int)
};

#endif