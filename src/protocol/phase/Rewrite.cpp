#include "Rewrite.hpp"

using namespace Wbsv;

int Rewrite::handler(Http& h)
{
	printLog(LOG_DEBUG, "Rewrite::handler");
	LocationCtx* lc = h.getLocationCtx();
	if (!(dynamic_cast<ReturnLocationCtx*>(lc)))
		return OK;
	return lc->contentHandler(h);
}
