#include "Content.hpp"

using namespace Wbsv;

int Content::handler(Http& h)
{
#ifdef DEBUG
	std::cout << "Content handler" << std::endl;
#endif
	LocationCtx* lc = h.getLocationCtx();
	return lc->contentHandler(h);
}
