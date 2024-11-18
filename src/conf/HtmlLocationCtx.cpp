#include "HtmlLocationCtx.hpp"

using namespace Wbsv;

void HtmlLocationCtx::setIndex(const std::string& index)
{
	index_ = index;
}

void HtmlLocationCtx::setRoot(const std::string& root)
{
	root_ = root;
}

const std::string& HtmlLocationCtx::getIndex(void) const
{
	return index_;
}
const std::string& HtmlLocationCtx::getRoot(void) const
{
	return root_;
}

static int autoindexHandler(Http& h, std::string& dirname)
{
	std::cout << "autoindexHandler" << std::endl;
	DIR* dirp = opendir(dirname.c_str());
	if (!dirp)
		return h.createResponse("404");
	struct dirent* dp;
	h.statusLine = "HTTP/1.1 200 OK\r\n";
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
		char buf[80];
		std::memset(buf, 0, 80);
		std::strftime(buf, 80, "%d-%m-%Y %H:%M", std::localtime(&sb.st_mtime));

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
	closedir(dirp);
	h.messageBodyOut += "</pre><hr>\n</body>\n</html>";
	std::stringstream size;
	size << h.messageBodyOut.size();
	h.headerOut += "Content-Type: text/html\r\n";
	h.headerOut += "Content-Length: " + size.str() + "\r\n";
	return DONE;
}

static int haveRightAccess(std::string& path, int method)
{
	if (method == DELETE)
	{
		int slashPos = 0;
		for (size_t i = 0; i < path.size(); i++)
		{
			// directory
			if (path[i] == '/')
			{
				slashPos = i;
				std::string dir(path.substr(0, i + 1));
				if (access(dir.c_str(), F_OK) == -1)
					return F_KO;
				if (access(dir.c_str(), X_OK) == -1)
				{
					std::cout << "X_KO: " << path.substr(0, i + 1) << std::endl;
					return WX_KO;
				}
			}
		}
		std::string dir(path.substr(0, slashPos + 1));
		if (access(dir.c_str(), W_OK) == -1)
		{
			std::cout << "W_KO: " << path.substr(0, slashPos + 1) << std::endl;
			return WX_KO;
		}
		return FRWX_OK;
	}
	for (size_t i = 0; i < path.size(); i++)
	{
		// directory
		if (path[i] == '/')
		{
			std::string dir(path.substr(0, i + 1));
			if (access(dir.c_str(), F_OK) == -1)
				return D_KO;
			if (access(dir.c_str(), X_OK) == -1)
				return X_KO;
		}
	}
	// file
	if (access(path.c_str(), F_OK) == -1)
		return F_KO;
	if (access(path.c_str(), R_OK) == -1)
		return R_KO;
	return FRWX_OK;
}

int HtmlLocationCtx::contentHandler(Http& h)
{

	printLog(LOG_DEBUG, "HtmlLocationCtx::contentHandler");
	std::string fullPath = root_;
	std::string uri = h.getUri();
	fullPath += uri;
	if (uri[uri.size() - 1] == '/')
	{
		std::string fullPathWithIndex = fullPath + index_;
		int rv = 0;
		if ((rv = haveRightAccess(fullPathWithIndex, h.getMethod())) != FRWX_OK)
		{
			if (!autoindex || h.getMethod() == DELETE || rv == X_KO ||
				rv ==
					D_KO) // even if autoindex == true, X_KO (dir does not have execute permission) means forbidden.
			{
				if (rv == D_KO && !h.notFound)
					return h.createResponse("404");
				else if (!h.forbidden)
					return h.createResponse("403");
				// if forbidden == true, it may cause loop infinite.
				h.statusLine = "HTTP/1.1 403 Forbidden\r\n";
				h.messageBodyOut = h.defaultErrorPages["403"];
				std::stringstream size;
				size << h.messageBodyOut;
				h.headerOut += "Content-Type: text/html\r\n";
				h.headerOut += "Content-Length: " + size.str() + "\r\n";
				return DONE;
			}
		}
		switch (h.getMethod())
		{
		case POST:
			return h.createResponse("405");
		case DELETE:
			if (std::remove(fullPath.c_str()) < 0)
				return h.createResponse("500");
			return h.createResponse("204");
		default: // GET
			int fd = open(fullPathWithIndex.c_str(), O_RDONLY);
#ifdef DEBUG
			std::stringstream tmp;
			tmp << fd;
			printLog(LOG_DEBUG, "file for response is opened (" + tmp.str() + ")");
#endif
			if (fd == -1)
			{
				if (!autoindex)
				{
					// Don't use h.createResponse(), it cause infinite loop, when error_page 500 specified.
					h.statusLine = "HTTP/1.1 500 Internal Server Error\r\n";
					h.messageBodyOut = h.defaultErrorPages["500"];
					std::stringstream size;
					size << h.messageBodyOut;
					h.headerOut += "Content-Type: text/html\r\n";
					h.headerOut += "Content-Length: " + size.str() + "\r\n";
					return DONE;
				}
				return autoindexHandler(h, fullPath);
			}
			if (!h.internalRedirect)
				h.statusLine = "HTTP/1.1 200 OK\r\n";
			size_t pos = fullPathWithIndex.rfind(".");
			if (pos == string::npos)
				h.headerOut += "Content-Type: text/plain\r\n";
			else
			{
				std::string ext(fullPathWithIndex.substr(pos + 1));
				if (ext == "html")
					h.headerOut += "Content-Type: text/html\r\n";
				else if (ext == "png")
					h.headerOut += "Content-Type: image/png\r\n";
				else if (ext == "jpeg")
					h.headerOut += "Content-Type: image/jpeg\r\n";
			}
			struct stat st;
			if (stat(fullPathWithIndex.c_str(), &st) == -1)
				return h.createResponse("500");
			std::stringstream size;
			size << st.st_size;
			h.headerOut += "Content-Length: " + size.str() + "\r\n";
			h.setFd(fd);
		}
	}
	else
	{
		int rv = 0;
		if ((rv = haveRightAccess(fullPath, h.getMethod())) != FRWX_OK)
		{
			if (rv == F_KO)
			{
				if (!h.notFound)
					return h.createResponse("404");
				// if notFound == true, it may cause loop infinite.
				h.statusLine = "HTTP/1.1 403 Forbidden\r\n";
				h.messageBodyOut = h.defaultErrorPages["403"];
				std::stringstream size;
				size << h.messageBodyOut;
				h.headerOut += "Content-Type: text/html\r\n";
				h.headerOut += "Content-Length: " + size.str() + "\r\n";
				return DONE;
			}
			if (!h.forbidden)
				return h.createResponse("403");
			// if forbidden == true, it may cause loop infinite.
			h.statusLine = "HTTP/1.1 403 Forbidden\r\n";
			h.messageBodyOut = h.defaultErrorPages["403"];
			std::stringstream size;
			size << h.messageBodyOut;
			h.headerOut += "Content-Type: text/html\r\n";
			h.headerOut += "Content-Length: " + size.str() + "\r\n";
			return DONE;
		}
		switch (h.getMethod())
		{
		case POST:
			return h.createResponse("405");
		case DELETE:
			if (std::remove(fullPath.c_str()) < 0)
				return h.createResponse("404");
			return h.createResponse("204");
		default:
			int fd = open(fullPath.c_str(), O_RDONLY);
#ifdef DEBUG
			std::stringstream tmp;
			tmp << fd;
			printLog(LOG_DEBUG, "file for response is opened (" + tmp.str() + ")");
#endif
			if (fd == -1)
			{
				// Don't use h.createResponse(), it cause infinite loop, when error_page 500 specified.
				h.statusLine = "HTTP/1.1 500 Internal Server Error\r\n";
				h.messageBodyOut = h.defaultErrorPages["500"];
				std::stringstream size;
				size << h.messageBodyOut;
				h.headerOut += "Content-Type: text/html\r\n";
				h.headerOut += "Content-Length: " + size.str() + "\r\n";
				return DONE;
			}
			if (!h.internalRedirect)
				h.statusLine = "HTTP/1.1 200 OK\r\n";
			size_t pos = fullPath.rfind(".");
			if (pos == string::npos)
				h.headerOut += "Content-Type: text/plain\r\n";
			else
			{
				std::string ext(fullPath.substr(pos + 1));
				if (ext == "html")
					h.headerOut += "Content-Type: text/html\r\n";
				else if (ext == "png")
					h.headerOut += "Content-Type: image/png\r\n";
				else if (ext == "jpeg")
					h.headerOut += "Content-Type: image/jpeg\r\n";
			}
			struct stat st;
			if (stat(fullPath.c_str(), &st) == -1)
				return h.createResponse("500");
			std::stringstream size;
			size << st.st_size;
			h.headerOut += "Content-Length: " + size.str() + "\r\n";
			h.setFd(fd);
		}
	}
	return DONE;
}
