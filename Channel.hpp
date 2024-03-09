#ifndef CHANNEL
#define CHANNEL

#include <iostream>
#include <vector>
#include <algorithm>
#include "Client.hpp"
#include <map>
class Client;

class Channel {
	private :
		std::string _channelName;
		std::vector <Client > _channel;
		std::string Channel_password; // the password of a channel
		// int limit_member; // max member of client can a channel hold
		std::map<int, std::string> _operator;
	public :
		bool _passSet; // true if +k is set in this channel
		bool _Invite; // true if +i is set in this channel
		bool _Topic; // true if +t is set in this channel
		bool _Oper; // true if +o is set in this channel
		bool _limit; // true if +l is set in this channel
		Channel();
		Channel(const Channel& obj);
		Channel& operator=(const Channel& obj);
		~Channel();
		// void setChannel(Client &cl);
		// getters and setters
		void setChannelName(std::string name);
		std::string getChannelName();
		std::vector<Client> &getChannel();
		void setOperator(int oper, std::string nickname);
		std::string getChannelName() const;
		bool isAMember(std::string nick);
		void eraseMember(std::string nick);
		// Client ret(int)
		std::string getChannelPassword() const;
		//member functions
		std::map<int, std::string> getOperator() const;
};

#endif