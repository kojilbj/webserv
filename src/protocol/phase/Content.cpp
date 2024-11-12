#include "Content.hpp"

using namespace Wbsv;

int Content::handler(Http& h)
{
	printLog(LOG_DEBUG, "Content::handler");
	LocationCtx* lc = h.getLocationCtx();
	return lc->contentHandler(h);
}
