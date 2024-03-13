#ifndef CHANNEL
#define CHANNEL

#include <iostream>
#include <vector>
#include <algorithm>
#include "Client.hpp"
#include <map>
class Client;

struct ChannelMode {
	bool _inviteOnly;
	bool _topic;
	bool _key;
	bool _opera;
	bool _limit;
	ChannelMode();
	bool allOn();
	bool allOff();
};

class Channel {
	private :
		std::string _channelName;
		std::vector <Client > _channel;
		std::string ChannelPwd; // the password of a channel
		int limitMbr; // max member of client can a channel hold
		std::string key;
		std::map<int, std::string> _operator;
		std::string channelTopic;
	public :
		ChannelMode _channelMode;
		Channel();
		Channel(const Channel& obj);
		Channel& operator=(const Channel& obj);
		~Channel();

		//----------Setter-----------
		void setChannelName(std::string name);
		void setOperator(int oper, std::string nickname);
		void setChannelPwd(std::string passw);
		void setlimitMbr(int nbr);
		void setKey(std::string key);
		void setChannelTopic(std::string topic);

		//----------Getter-----------
		std::string getChannelName();
		std::string getChannelName() const;
		std::string getChannelPassword() const;
		std::vector<Client> &getChannel();
		std::map<int, std::string> getOperator() const;
		std::string getChannelPwd() const;
		int getlimitMbr() const;
		std::string getKey() const;
		std::string getChannelTopic() const;

		//---------------------------
		int isOperator(std::string nickname);
		bool isAMember(std::string nick);
		void eraseMember(std::string nick);
		void eraseOperator(int fd);
};

#endif