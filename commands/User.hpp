#ifndef USER_HPP
#define USER_HPP


#include "../InfoClient.hpp"

class User : public InfoClient
{
public:
	User();
	~User();

	static void executeUser(std::vector<std::string> &vec);
};


#endif