#include "Content.hpp"

using namespace Wbsv;

int Content::handler(Http& h)
{
#ifdef DEBUG
	std::cout << "Content handler" << std::endl;
#endif
	return OK;
}
