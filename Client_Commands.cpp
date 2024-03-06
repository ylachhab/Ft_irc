#include "Client.hpp"
#include "Server.hpp"

std::vector<Client> Server::cObjs;
std::vector <Channel >Server::_channels;

/******************* PASS Command **********************/
void Client::executePass(std::vector<std::string> &vec)
{
	if (this->_registred)
		std::cout << "PASS :You may not reregister\n";
	else
	{
		if (vec.size() && !vec[0].empty())
		{
			this->_pass = true;
			if (vec[0].compare(this->_password) == 0)
				this->_authenticated = true;
			else
				this->_authenticated = false;
		}
		else
			std::cerr << "PASS :Not enough parameters\n";
	}
}


/******************* NICK Command **********************/
bool specialCharacter(std::string &str)
{
	std::string sp = "-[]\\'^{}";
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (!std::isalpha(*it) && !std::isdigit(*it) && sp.find(*it) == std::string::npos)
			return true;
	}
	return false;
}


void Client::executeNick(std::vector<std::string> &vec)
{
	if (this->_registred)
		std::cout << "NICK :You may not reregister\n";
	for (size_t i = 0; i < Server::cObjs.size(); i++)
	{
		if (Server::cObjs[i]._nickName == vec[0])
		{
			std::cerr << vec[0] << " :Nickname is already in use.\n";
			return ;
		}
	}
	if (this->_pass)
	{
		if (vec.size() && !vec[0].empty())
		{
			if (specialCharacter(vec[0]) == 0)
			{
				this->_nick = true;
				this->_nickName = vec[0];
				std::cout << "your nickname is: " << this->_nickName << "\n";
			}
			else
				std::cerr << "NICK :invalid nickname is given\n";
		}
		else
			std::cerr << "NICK :No nickname given\n";
	}
}

/******************* USER Command **********************/
void Client::executeUser(std::vector<std::string> &vec)
{
	if (this->_registred)
		std::cout << "USER :You may not reregister\n";
	if (this->_pass && this->_nick)
	{
		if (vec.size() >= 4)
		{
			this->_userName = vec[0];
			this->_realName = vec[3];
			this->_user = true;
			this->_registred = true;
			std::cout << "the user " << this->_userName << " was successfully regestred ";
			if (this->_authenticated)
				std::cout << "and authenticated!\n";
			else
				std::cout << "but not authenticated!\n";
		}
		else
			std::cerr << "USER :Not enough parameters\n";
	}
}

/******************* JOIN Command **********************/
// void Client::executeJoin(std::vector<std::string> &vec)
// {
	
// }