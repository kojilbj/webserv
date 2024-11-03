#include "ReturnLocationCtx.hpp"

using namespace Wbsv;

int ReturnLocationCtx::contentHandler(Http& h)
{
	h.statusLine += "HTTP/1.1 " + redirect.first + " Moved Parmanently\r\n";
	if (redirect.first == "301" || redirect.first == "302" || redirect.first == "303" ||
		redirect.first == "307")
		h.headerOut += "Location: " + redirect.second + "\r\n";
	h.headerOut += "\r\n";
	h.messageBodyOut += redirect.second;
	/* if ( rewrite destination is like "http://www.google.com" ) */
	/* redirect response */
	/* if ( rewrite destination is like "localhost:80/something" ) */
	/* internal redirect (jump to FindConfig Phase) */
	/* count redirect (redirect loop is up to 5) */
	return DONE;
}