#include "HtmlLocationCtx.hpp"

using namespace Wbsv;

static int autoindexHandler(Http& h, std::string& dirname)
{
	std::cout << "autoindexHandler" << std::endl;
	DIR* dirp = opendir(dirname.c_str());
	if (!dirp)
	{
		h.statusLine = "HTTP/1.1 500 Internal Server Error\r\n";
		h.headerOut = "\r\n";
		return DONE;
	}
	struct dirent* dp;
	h.statusLine = "HTTP/1.1 200 OK\r\n";
	h.headerOut += "\r\n";
	h.messageBodyOut += "<html>\n<head>\n<title>Index of " + h.getUri() +
						"</title></head>\n<body>\n<h1>Index of " + h.getUri() +
						"</h1>\n<hr><pre>\n";
	size_t dirMaxLen = 80;
	for (;;)
	{
		dp = readdir(dirp);
		if (!dp)
			break;
		std::string dirTmp(dp->d_name);
		std::string fullPath = dirname + dirTmp;
		struct stat sb;
		if (stat(fullPath.c_str(), &sb) == -1)
			break;
		// get last modified time of directory or file.
		char buf[20];
		std::memset(buf, 0, 20);
		std::strftime(buf, 19, "%d-%m-%Y %H:%M", std::localtime(&sb.st_mtime));

		if (dp->d_type == DT_DIR)
			dirTmp += "/";
		std::string ref = "<a href=\"" + dirTmp + "\">" + dirTmp + "</a>";
		int spLen = dirMaxLen - dirTmp.size();
		std::string spaces(spLen > 0 ? spLen : 1, ' ');
		if (dp->d_type == DT_DIR)
			h.messageBodyOut += ref + spaces + buf + "     -\n";
		else
		{
			std::stringstream size;
			size << sb.st_size;
			h.messageBodyOut += ref + spaces + buf + "     " + size.str() + "\n";
		}
	}
	h.messageBodyOut += "</pre><hr>\n</body>\n</html>";
	return DONE;
}

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
			if (!autoindex)
			{
				h.statusLine = "HTTP/1.1 404 Not Found\r\n";
				h.headerOut = "\r\n";
				h.messageBodyOut = h.defaultErrorPages["404"];
				return DONE;
			}
			return autoindexHandler(h, fullPath);
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