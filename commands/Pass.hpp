#ifndef PASS_HPP
#define PASS_HPP


#include "Client.hpp"

class Pass : public Client
{
public:
	Pass();
	~Pass();

	void executeCmd(std::vector<std::string> &vec);
};


#endif
