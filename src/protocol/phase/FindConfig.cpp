#include "FindConfig.hpp"

using namespace Wbsv;

int FindConfig::checker(Http&)
{
	std::cout << "FindConfig checker" << std::endl;
	return DONE;
}

void FindConfig::handler(Http&) { }
