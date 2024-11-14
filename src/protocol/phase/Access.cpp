#include "Access.hpp"

using namespace Wbsv;

int Access::handler(Http& h)
{
	(void)h;
	printLog(LOG_DEBUG, "Access::handler");
	LocationCtx* lc = h.getLocationCtx();
	switch (h.getMethod())
	{
	case GET:
		if (!lc->allowGet())
		{
			h.createResponse("405");
			return DONE;
		}
		break;
	case POST:
		if (!lc->allowPost())
		{
			h.createResponse("405");
			return DONE;
		}
		break;
	case DELETE:
		if (!lc->allowDelete())
		{
			h.createResponse("405");
			return DONE;
		}
		break;
	}
	return OK;
}
