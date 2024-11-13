#include "Access.hpp"

using namespace Wbsv;

int Access::handler(Http& h)
{
	(void)h;
	printLog(LOG_DEBUG, "Access::handler");
	// LocationCtx* lc = h.getLocationCtx();
	// switch (h.getMethod())
	// {
	// case GET:
	// 	if (!lc->getLimitExcept(GET))
	// 	{
	// 		h.statusLine = "HTTP/1.1 405 Not Allowed\r\n";
	// 		h.headerOut = "Allow: ";
	// 		if (lc->getLimitExcept(POST))
	// 			h.headerOut += "POST";
	// 		if (lc->getLimitExcept(DELETE))
	// 			h.headerOut += ", DELETE";
	// 		h.headerOut += "\r\n";
	// 		return DONE;
	// 	}
	// 	break;
	// case POST:
	// 	if (!lc->getLimitExcept(POST))
	// 	{
	// 		h.statusLine = "HTTP/1.1 405 Not Allowed\r\n";
	// 		h.headerOut = "Allow: ";
	// 		if (lc->getLimitExcept(GET))
	// 			h.headerOut += "GET";
	// 		if (lc->getLimitExcept(DELETE))
	// 			h.headerOut += ", DELETE";
	// 		h.headerOut += "\r\n";
	// 		return DONE;
	// 	}
	// 	break;
	// case DELETE:
	// 	if (!lc->getLimitExcept(DELETE))
	// 	{
	// 		h.statusLine = "HTTP/1.1 405 Not Allowed\r\n";
	// 		h.headerOut = "Allow: ";
	// 		if (lc->getLimitExcept(GET))
	// 			h.headerOut += "GET";
	// 		if (lc->getLimitExcept(POST))
	// 			h.headerOut += ", POST";
	// 		h.headerOut += "\r\n";
	// 		return DONE;
	// 	}
	// 	break;
	// }
	return OK;
}
