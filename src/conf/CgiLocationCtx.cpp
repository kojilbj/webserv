#include "CgiLocationCtx.hpp"

using namespace Wbsv;

static void parseUri(Http& h, std::map<std::string, std::string>& param, std::string index)
{
	int pos = 0;
	std::string uri = h.getUri();
	std::string path;
	if ((pos = uri.find("?")) != string::npos)
	{
		path = uri.substr(0, pos);
		int pos2 = 0;
		if ((pos2 = uri.find("#", pos + 1)) != string::npos)
			param["QUERY_STRING"] = uri.substr(pos + 1, pos2);
		else
			param["QUERY_STRING"] = uri.substr(pos + 1);
	}
	else
		path = uri;
	std::string phpExt = ".php";
	std::string pathInfo;
	pos = path.find(phpExt);
	if (pos != string::npos && path[pos + phpExt.size()] != '\0')
	{
		// it conflict with PATH_INFO of RFC3875(CGI), but subject's requirement.
		pathInfo = path.substr(0, pos + phpExt.size());
	}
	else
		pathInfo = path;
	if (pathInfo[pathInfo.size() - 1] == '/')
		param["PATH_INFO"] = uri + index;
	else
		param["PATH_INFO"] = uri;
	std::string scriptFilename = param["SCRIPT_FILENAME"];
	std::string pathInfoVar = "$cgi_path_info";
	pos = scriptFilename.find(pathInfoVar);
	if (pos != string::npos)
	{
		scriptFilename.replace(pos, pathInfoVar.size(), pathInfo);
		param["SCRIPT_FILENAME"] = scriptFilename;
	}
}

static void headerIn2Param(Http& h, std::map<std::string, std::string>& param)
{
	std::map<std::string, std::string>& headersIn = h.getHeadersIn();
	std::map<std::string, std::string>::iterator it;
	it = headersIn.find("Content-Type");
	if (it != headersIn.end())
		param["CONTENT_TYPE"] = headersIn["Content-Type"];
	it = headersIn.find("Content-Length");
	if (it != headersIn.end())
		param["CONTENT_LENGTH"] = headersIn["Content-Length"];
	int method = h.getMethod();
	switch (method)
	{
	case GET:
		param["REQUEST_METHOD"] = "GET";
		break;
	case POST:
		param["REQUEST_METHOD"] = "POST";
		break;
	}
}

static size_t maxParamLen(std::map<std::string, std::string>& param)
{
	size_t max = 0;
	std::map<std::string, std::string>::iterator it = param.begin();
	for (; it != param.end(); it++)
	{
		size_t len = it->first.size() + it->second.size();
		if (max < len)
			max = len;
	}
	return max;
}

int CgiLocationCtx::contentHandler(Http& h)
{
	parseUri(h, param, index);
	headerIn2Param(h, param);
#ifdef DEBUG
	std::cout << "Cgi contentHandler" << std::endl;
	std::map<std::string, std::string>::iterator itDebug = param.begin();
	for (; itDebug != param.end(); itDebug++)
	{
		std::cout << "{ " << itDebug->first << " : " << itDebug->second << " }" << std::endl;
	}
#endif
	char environ[param.size() + 1][maxParamLen(param) + 1];
	std::memset(environ, 0, sizeof(environ));
	std::map<std::string, std::string>::iterator it = param.begin();
	for (int i = 0; i != param.size(); i++, it++)
	{
		std::string value = it->first + "=" + it->second;
		std::strncpy(environ[i], value.c_str(), value.size());
	}
	std::string pathname;
	std::string arg1;
	// if (param["PATH_INFO"].find(".php") != string::npos)
	// {
	pathname = "/usr/bin/php-cgi";
	arg1 = param["PATH_INFO"];
	// }
	char* argv[3];
	argv[0] = pathname.c_str();
	argv[1] = arg1.c_str();
	argv[2] = NULL;
	int pfd[2];
	if (pipe(pfd) < 0)
	{
		// Internal Server Error;
		return DONE;
	}
	int pid = fork();
	if (pid < 0)
	{
		// Internal Server Error;
		return DONE;
	}
	if (pid == 0)
	{
		// child process
		close(pfd[1]);
		dup2(pfd[0], STDIN_FILENO);
		execve(argv[0], argv, environ);
		std::cerr << strerror(errno) << std::endl;
	}
	else
	{
		close(pfd[0]);
		h.setFd(dup(STDOUT_FILENO));
		std::string headerIn = h.getHeaderIn();
		int pos = h.getPos();
		ssize_t writenum = write(pfd[1], headerIn.c_str() + pos, headerIn.size() - pos);
		if (not all of request message read)
			return h.readEventHandler();
		// for (;;)
		// {
		// 	char buf[];
		// 	read(h.c.fd, );
		// 	ssize_t writenum = write(pfd[1], headerIn.c_str() + pos, headerIn.size() - pos);
		// }
		close(pfd[1]);
	}
	return DONE;
}