#include "FindConfig.hpp"

using namespace Wbsv;

int FindConfig::checker(Http& h)
{
	std::cout << "FindConfig checker" << std::endl;
	return handler(h);
}

int FindConfig::handler(Http& h)
{
	VServerCtx* vs = h.getVServerCtx();
	std::vector<LocationCtx>* lv = vs->getLocationCtxs();
	std::vector<LocationCtx>::iterator it;
	bool found = false;
	for (it = lv->begin(); it != lv->end(); it++)
	{
		std::string requestUri = h.getUri();
		std::string path = it->path;
		if (path.compare(requestUri.substr(0, path.size())) == 0)
		{
			found = true;
			h.setLocationCtx(&(*it));
		}
	}
	if (!found)
	{
		std::cout << "404 Not Found" << std::endl;
		return ERROR;
	}
#ifdef DEBUG
	std::cout << "LocationCtx found" << std::endl;
	std::cout << "path: " << h.getLocationCtx()->path << std::endl;
	std::cout << "root: " << h.getLocationCtx()->root << std::endl;
	std::cout << "index: " << h.getLocationCtx()->index << std::endl;
#endif
	return OK;
}
