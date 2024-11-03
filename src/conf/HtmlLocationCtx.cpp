#include "HtmlLocationCtx.hpp"

using namespace Wbsv;

int HtmlLocationCtx::contentHandler(Http& h)
{
	std::string fullPath = root;
	std::string uri = h.getUri();
	fullPath += uri;
	if (uri[uri.size() - 1] == '/')
	{
		std::string fullPathWithIndex = fullPath + index;
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
			h.messageBodyOut = h.defaultErrorPages["404"];
			return DONE;
		}
		h.statusLine = "HTTP/1.1 200 OK\r\n";
		h.headerOut = "\r\n";
		h.setFd(fd);
	}
	return DONE;
}