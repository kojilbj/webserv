#include "Access.hpp"

using namespace Wbsv;

int Access::handler(Http& h)
{
#ifdef DEBUG
	std::cout << "Access handler" << std::endl;
#endif
	// switch (h.getMethod())
	// {
	// case GET:
	// 	if (!getLimitExcept(GET))
	// 	{
	// 		h.statusLine = "HTTP/1.1 405 Not Allowed\r\n";
	// 		h.headerOut = "Allow: ";
	// 		if (getLimitExcept(POST))
	// 			h.headerOut += "POST";
	// 		if (getLimitExcept(DELETE))
	// 			h.headerOut += ", DELETE";
	// 		return DONE;
	// 	}
	// 	break;
	// case POST:
	// 	if (!getLimitExcept(POST))
	// 	{
	// 		h.statusLine = "HTTP/1.1 405 Not Allowed\r\n";
	// 		h.headerOut = "Allow: ";
	// 		if (getLimitExcept(GET))
	// 			h.headerOut += "GET";
	// 		if (getLimitExcept(DELETE))
	// 			h.headerOut += ", DELETE";
	// 		return DONE;
	// 	}
	// 	break;
	// case DELETE:
	// 	if (!getLimitExcept(DELETE))
	// 	{
	// 		h.statusLine = "HTTP/1.1 405 Not Allowed\r\n";
	// 		h.headerOut = "Allow: ";
	// 		if (getLimitExcept(GET))
	// 			h.headerOut += "GET";
	// 		if (getLimitExcept(POST))
	// 			h.headerOut += ", POST";
	// 		return DONE;
	// 	}
	// 	break;
	// }
	return OK;
}
