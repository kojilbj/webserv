#include "Content.hpp"

using namespace Wbsv;

int htmlContentHandler(Http& h, LocationCtx* lc)
{
	std::string fullPath = lc->root;
	std::string uri = h.getUri();
	fullPath += uri;
	if (uri[uri.size() - 1] == '/')
	{
		std::string fullPathWithIndex = fullPath + lc->index;
		int fd = open(fullPathWithIndex.c_str(), O_RDONLY);
		if (fd == -1)
		{
			// autoindex
			std::cout << "we will display directory list" << std::endl;
			return DONE;
		}
		h.statusLine = "HTTP/1.1 200 OK\r\n";
		h.headerOut = "\r\n";
		h.setFd(fd);
	}
	else
	{
		int fd = open(fullPath.c_str(), O_RDONLY);
		if (fd == -1)
		{
			h.statusLine = "HTTP/1.1 404 Not Found\r\n";
			h.headerOut = "\r\n";
			return DONE;
		}
		h.statusLine = "HTTP/1.1 200 OK\r\n";
		h.headerOut = "\r\n";
		h.setFd(fd);
	}
	return DONE;
}

int Content::handler(Http& h)
{
#ifdef DEBUG
	std::cout << "Content handler" << std::endl;
#endif
	LocationCtx* lc = h.getLocationCtx();
	if (lc->name == "html") // && method == GET ???
		return htmlContentHandler(h, lc);
	return OK;
}
