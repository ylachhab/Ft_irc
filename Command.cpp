#include "Client.hpp"


void Client::Kick() {
	if (this->_authenticated && this->_registed)
	{
		if (vec.size() > 2)
		{
			std::string str = vec[0];
			if (str[0] == '#' && str.length() > 2)
				vec[0].substr(1);
			else {
				std::cout << vec[0] << " :No such channel" << std::endl;
				return ;
			}
			for (size_t i = 0; i < Server::_channels.size(); i++)
			{
				if (Server::_channels[i].getChannelName() == vec[0])
				{
					if (Server::_channels[i].getOperator() == this->_fd)
					{
						if (Server::_channels[i].isAMember(vec[1]))
						{
							Server::_channels[i].eraseMember(vec[1]);
							if (vec.size() > 3)
								std::cout << "KICK #" << Server::_channels[i].getChannelName()
									<< vec[vec.size() - 1] << " :" << vec[vec.size() - 1] << std::endl;
							else
								std::cout << "KICK #" << Server::_channels[i].getChannelName()
									<< vec[1] << " :" << vec[1] << std::endl;
						}
						else {
							std::cout << ":They aren't on that channel" << std::endl;
							return ;
						}
					}
					else {
						std::cout << _fd << " :You're not channel operator" << std::endl;
						return ;
					}
					
				}
			}
		}
		else {
			std::cout << "Kick :Not enough parameters" << std::endl;
			return ;
		}
	}
	else
		std::cout << " :Register first." << std::endl;
}

void Client::Invite() {
	if (this->_authenticated && this->_registed)
	{
		if (vec.size() > 2)
		{
			for (size_t i = 0; i < Server::cObjs.size(); i++)
			{
				if (Server::cObjs[i].getNickName() == vec[0])
				{
				}
				else {
					std::cout << this->_nickName << vec[0] << " :No such nick" << std::endl;
					return ;
				}
			}
		}
		else {
			std::cout << this->_nickName << " INVITE :Not enough parameters" << std::endl;
		}
	}
}