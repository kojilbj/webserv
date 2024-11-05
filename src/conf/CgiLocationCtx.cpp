#include "CgiLocationCtx.hpp"

using namespace Wbsv;

static void
parseUri(Http& h, std::map<std::string, std::string>& param, std::string index, std::string store)
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
	if (pos != string::npos && pos + phpExt.size() < path.size())
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
		scriptFilename.replace(pos, pathInfoVar.size(), param["PATH_INFO"]);
		param["SCRIPT_FILENAME"] = scriptFilename;
	}
	// if (store[store.size() - 1] != '/')
	// 	store += "/";
	// param["UPLOAD_DIR"] = store;
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
		size_t len = it->first.size() + it->second.size() + 1; // 1 means "=".
		if (max < len)
			max = len;
	}
	return max;
}

int CgiLocationCtx::contentHandler(Http& h)
{
	parseUri(h, param, index, store);
	headerIn2Param(h, param);
#ifdef DEBUG
	std::cout << "Cgi contentHandler" << std::endl;
	std::map<std::string, std::string>::iterator itDebug = param.begin();
	for (; itDebug != param.end(); itDebug++)
	{
		std::cout << "{ " << itDebug->first << " : " << itDebug->second << " }" << std::endl;
	}
#endif
	char environData[param.size()][maxParamLen(param) + 1];
	std::memset(environData, 0, sizeof(environData));
	std::map<std::string, std::string>::iterator it = param.begin();
	for (int i = 0; i != param.size(); i++, it++)
	{
		std::string value = it->first + "=" + it->second;
		std::strncpy(environData[i], value.c_str(), value.size());
	}
	char* environPtrs[param.size()];
	int i = 0;
	for (; i != param.size(); i++)
	{
		environPtrs[i] = environData[i];
	}
	environPtrs[i] = NULL;
	char** environ = environPtrs;
	std::string pathname;
	std::string arg1;
	// if (param["PATH_INFO"].find(".php") != string::npos)
	// {
	pathname = "/usr/bin/php-cgi";
	arg1 = param["PATH_INFO"];
	// }
	char* argv[3];
	argv[0] = const_cast<char*>(pathname.c_str());
	argv[1] = const_cast<char*>(arg1.c_str());
	argv[2] = NULL;
	// fd to send from parent to child.
	int p2cFd[2];
	if (pipe(p2cFd) < 0)
	{
		// Internal Server Error;
		return DONE;
	}
	int c2pFd[2];
	if (pipe(c2pFd) < 0)
	{
		// Internal Server Error;
		close(p2cFd[0]);
		close(p2cFd[1]);
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
		std::cout << "argv[0]: " << argv[0] << std::endl;
		std::cout << "argv[1]: " << argv[1] << std::endl;
		// child process
		close(p2cFd[1]);
		dup2(p2cFd[0], STDIN_FILENO);
		close(c2pFd[0]);
		dup2(c2pFd[1], STDOUT_FILENO);
		char** pEnvrion = environ;
		execve(argv[0], argv, pEnvrion);
		std::cerr << "execve: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		close(p2cFd[0]);
		close(c2pFd[1]);
		h.upstream = new Upstream;
		h.upstream->writeFd = p2cFd[1];
		h.upstream->readFd = c2pFd[0];
		h.upstream->p = reinterpret_cast<Protocol*>(&h);
		// int fd = open(h.getRequestBodyFileName().c_str(), O_RDONLY);
		// for (;;)
		// {
		// 	size_t bufSize = 1024;
		// 	char buf[bufSize + 1];
		// 	std::memset(buf, 0, bufSize + 1);
		// 	ssize_t readnum = read(fd, buf, bufSize);
		// 	if (readnum <= 0)
		// 		break;
		// 	// maybe block
		// 	ssize_t writenum = write(p2cFd[1], buf, readnum);
		// 	if (writenum < 0)
		// 		return ERROR;
		// }
		// close(fd);
		// close(p2cFd[1]);
		// std::remove(h.getRequestBodyFileName().c_str());

		// #ifdef DEBUG
		// 		std::cout << "-----------------------" << std::endl;
		// 		std::cout << "Cgi executed" << std::endl << "result:" << std::endl;
		// 		size_t bufSize = 1024;
		// 		char cgiBuf[bufSize + 1];
		// 		std::memset(cgiBuf, 0, bufSize + 1);
		// 		ssize_t cgiReadnum = read(c2pFd[0], cgiBuf, bufSize);
		// 		std::cout << "readnum: " << cgiReadnum << std::endl;
		// 		std::cout << "buf: " << cgiBuf << std::endl;
		// 		std::cout << "-----------------------" << std::endl;
		// #endif
	}
	return DONE;
}