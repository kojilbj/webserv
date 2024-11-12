#include "CgiLocationCtx.hpp"

using namespace Wbsv;
CgiLocationCtx::CgiLocationCtx(void)
{
#ifdef DEBUG
	std::cout << "CgiLocationCtx Constructor Called" << std::endl;
#endif
	index_ = "index.php";
	store_ = "/test";
}
CgiLocationCtx::CgiLocationCtx(const CgiLocationCtx& other)
	: LocationCtx(other), index_(other.index_), param_(other.param_), store_(other.store_)
{ }

CgiLocationCtx::~CgiLocationCtx(void) { }

CgiLocationCtx& CgiLocationCtx::operator=(const CgiLocationCtx& other)
{
	LocationCtx::operator=(other);
	index_ = other.index_;
	param_ = other.param_;
	store_ = other.store_;
	return *this;
}

void CgiLocationCtx::setIndex(const std::string& cgiIndex)
{
	if (cgiIndex.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Cgi Index: " + cgiIndex);
	index_ = cgiIndex;
}

const std::string& CgiLocationCtx::getIndex(void) const
{
	return index_;
}

void CgiLocationCtx::setParam(const std::string& key, const std::string& path)
{
	if (key.empty() || key.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Cgi Param Key: " + key);
	if (path.empty() || path.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Cgi Param Path: " + path);
	param_[key] = path;
}
void CgiLocationCtx::setStore(const std::string& path)
{
	if (path.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Cgi Index: " + path);
	store_ = path;
}
const std::string& CgiLocationCtx::getStore(void) const
{
	return store_;
}
std::map<std::string, std::string>& CgiLocationCtx::getParam(void)
{
	return param_;
}

static void
parseUri(Http& h, std::map<std::string, std::string>& param, std::string index, std::string store)
{
	size_t pos = 0;
	std::string uri = h.getUri();
	std::string path;
	if ((pos = uri.find("?")) != string::npos)
	{
		path = uri.substr(0, pos);
		size_t pos2 = 0;
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
	std::cout << "first scriptFilename" << scriptFilename << std::endl;
	std::string pathInfoVar = "$cgi_path_info";
	pos = scriptFilename.find(pathInfoVar);
	if (pos != string::npos)
	{
		scriptFilename.replace(pos, pathInfoVar.size(), param["PATH_INFO"]);
		param["SCRIPT_FILENAME"] = scriptFilename;
	}
	if (store[store.size() - 1] != '/')
		store += "/";
	param["HTTP_UPLOADDIR"] = store;
	param["SERVER_PROTOCOL"] = "HTTP/1.1";
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
	it = headersIn.find("Cookie");
	if (it != headersIn.end())
		param["HTTP_COOKIE"] = headersIn["Cookie"];
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
	std::string scriptFileNameTmp = param_["SCRIPT_FILENAME"];
	parseUri(h, param_, index_, store_);
	headerIn2Param(h, param_);
#ifdef DEBUG
	std::cout << "Cgi contentHandler" << std::endl;
	std::map<std::string, std::string>::iterator itDebug = param_.begin();
	for (; itDebug != param_.end(); itDebug++)
	{
		std::cout << "{ " << itDebug->first << " : " << itDebug->second << " }" << std::endl;
	}
#endif
	char environData[param_.size()][maxParamLen(param_) + 1];
	std::memset(environData, 0, sizeof(environData));
	std::map<std::string, std::string>::iterator it = param_.begin();
	for (size_t i = 0; i != param_.size(); i++, it++)
	{
		std::string value = it->first + "=" + it->second;
		std::strncpy(environData[i], value.c_str(), value.size());
	}
	char* environPtrs[param_.size()];
	size_t i = 0;
	for (; i != param_.size(); i++)
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
	arg1 = param_["PATH_INFO"];
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
		param_["SCRIPT_FILENAME"] = scriptFileNameTmp;
		if (param_.find("HTTP_COOKIE") != param_.end())
			param_.erase("HTTP_COOKIE");
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