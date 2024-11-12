#include "FindConfig.hpp"

using namespace Wbsv;

int FindConfig::handler(Http& h)
{
	printLog(LOG_DEBUG, "FindConfig::handler");
	VServerCtx* vs = h.getVServerCtx();
	std::vector<LocationCtx*>* lv = vs->getLocationCtxs();
	std::vector<LocationCtx*>::iterator it;
	bool found = false;
	for (it = lv->begin(); it != lv->end(); it++)
	{
		std::string requestUri = h.getUri();
		std::string path = (*it)->getPath();
		if (path.compare(requestUri.substr(0, path.size())) == 0)
		{
			found = true;
			h.setLocationCtx(*it);
		}
	}
	if (!found)
	{
		ErrorPages e = vs->getErrorPages();
		std::string path;
		if ((path = e.getErrorPagePath("404")) != "")
		{
			std::cout << "404 has default error page" << std::endl;
			h.setUri(path);
			return AGAIN;
		}
		std::cout << "404 has \"not\" default error page" << std::endl;
		h.statusLine = "HTTP/1.1 404 Not Found\r\n";
		h.headerOut = "\r\n";
		h.messageBodyOut = h.defaultErrorPages["404"];
		return DONE;
	}
	printLog(LOG_DEBUG, "LocationCtx found at " + h.getLocationCtx()->getPath());
	return OK;
}
