#include "Content.hpp"

using namespace Wbsv;

int Content::checker(Http& h)
{
	std::cout << "Content checker" << std::endl;
	return handler(h);
}

int Content::handler(Http& h)
{
	return OK;
}
