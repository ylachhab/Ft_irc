#ifndef PASS_HPP
#define PASS_HPP


#include "../InfoClient.hpp"

class Pass : public InfoClient
{
public:
	Pass();
	~Pass();

	static void executePass(std::vector<std::string> &vec);
};


#endif
