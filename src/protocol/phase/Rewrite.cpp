#include "Rewrite.hpp"

using namespace Wbsv;

int Rewrite::handler(Http& h)
{
#ifdef DEBUG
	std::cout << "Rewrite handler" << std::endl;
#endif
	LocationCtx* lc = h.getLocationCtx();
	if (lc->name != "redirect")
		return OK;
	h.headerOut += "HTTP/1.1 " + lc->redirect[0] + " Moved Parmanently\r\n";
	if (lc->redirect[0] == "301" || lc->redirect[0] == "302" || lc->redirect[0] == "303" ||
		lc->redirect[0] == "307")
		h.headerOut += "Location: " + lc->redirect[1] + "\r\n";
	h.headerOut += "\r\n";
	h.headerOut += lc->redirect[1];
	/* if ( rewrite destination is like "http://www.google.com" ) */
	/* redirect response */
	/* if ( rewrite destination is like "localhost:80/something" ) */
	/* internal redirect (jump to FindConfig Phase) */
	/* count redirect (redirect loop is up to 5) */
	return DONE;
}
