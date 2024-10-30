#include "FindConfig.hpp"

using namespace Wbsv;

int FindConfig::handler(Http& h)
{
#ifdef DEBUG
	std::cout << "FindConfig handler" << std::endl;
#endif
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
