#include "Rewrite.hpp"

using namespace Wbsv;

int Rewrite::handler(Http& h)
{
#ifdef DEBUG
	std::cout << "Rewrite handler" << std::endl;
#endif
	LocationCtx* lc = h.getLocationCtx();
	if (!(dynamic_cast<ReturnLocationCtx*>(lc)))
		return OK;
	return lc->contentHandler(h);
}
