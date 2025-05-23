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
	else
		param["CONTENT_TYPE"] = "text/plain";
	it = headersIn.find("Content-Length");
	if (it != headersIn.end())
		param["CONTENT_LENGTH"] = headersIn["Content-Length"];
	else
		param["CONTENT_LENGTH"] = "0";
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

static int haveRightAccess(std::string& path)
{
	for (size_t i = 0; i < path.size(); i++)
	{
		// directory
		if (path[i] == '/')
		{
			std::string dir(path.substr(0, i + 1));
			if (access(dir.c_str(), F_OK) == -1)
				return F_KO;
			if (access(dir.c_str(), R_OK | X_OK) == -1)
				return RX_KO;
		}
	}
	// file
	if (access(path.c_str(), F_OK) == -1)
		return F_KO;
	return FRX_OK;
}

int CgiLocationCtx::contentHandler(Http& h)
{
#ifdef DEBUG
	printLog(LOG_DEBUG, "CgiLocationCtx::contentHandler");
#endif
	std::string scriptFileNameTmp = param_["SCRIPT_FILENAME"];
	parseUri(h, param_, index_, store_);
	headerIn2Param(h, param_);
	int rv = haveRightAccess(param_["SCRIPT_FILENAME"]);
	if (rv != FRX_OK)
	{
		std::remove(h.requestBodyFileName_.c_str());
		if (rv == F_KO)
		{
			h.messageBodyOut = "File not found.\n";
			param_["SCRIPT_FILENAME"] = scriptFileNameTmp;
			return h.createResponse("404");
		}
		else // RX_KO
		{
			h.messageBodyOut = "Access denied.\n";
			param_["SCRIPT_FILENAME"] = scriptFileNameTmp;
			return h.createResponse("403");
		}
	}
#ifdef DEBUG
	printLog(LOG_DEBUG, "cgi environ created");
	std::map<std::string, std::string>::iterator itDebug = param_.begin();
	for (; itDebug != param_.end(); itDebug++)
	{
		std::string msg("{ ");
		msg += itDebug->first + " : " + itDebug->second + " }\n";
	}
#endif
	size_t pos = param_["SCRIPT_FILENAME"].rfind(".");
	if (pos == string::npos)
	{
		std::remove(h.requestBodyFileName_.c_str());
		param_["SCRIPT_FILENAME"] = scriptFileNameTmp;
		return h.createResponse("502");
	}
	std::string pathname;
	std::string ext(param_["SCRIPT_FILENAME"].substr(pos + 1));
	if (ext == "php")
	{
		//pathname = "/home/kisobe/.brew/bin/php-cgi";
		pathname = "./php-cgi";
		//pathname = "/usr/bin/php-cgi";
	}
	// else if (ext == "py")
	// 	pathname = "/usr/bin/python3";
	else
	{
		std::remove(h.requestBodyFileName_.c_str());
		param_["SCRIPT_FILENAME"] = scriptFileNameTmp;
		return h.createResponse("502");
	}

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
	std::string arg1 = param_["PATH_INFO"];
	char* argv[3];
	argv[0] = const_cast<char*>(pathname.c_str());
	argv[1] = const_cast<char*>(arg1.c_str());
	argv[2] = NULL;
	// fd to send from parent to child.
	int p2cFd[2];
	if (pipe(p2cFd) < 0)
	{
		std::remove(h.requestBodyFileName_.c_str());
		param_["SCRIPT_FILENAME"] = scriptFileNameTmp;
		return h.createResponse("500");
	}
	int c2pFd[2];
	if (pipe(c2pFd) < 0)
	{
		std::remove(h.requestBodyFileName_.c_str());
		close(p2cFd[0]);
		close(p2cFd[1]);
		param_["SCRIPT_FILENAME"] = scriptFileNameTmp;
		return h.createResponse("500");
	}
	int pid = fork();
	if (pid < 0)
	{
		// Internal Server Error;
		std::remove(h.requestBodyFileName_.c_str());
		close(p2cFd[0]);
		close(p2cFd[1]);
		close(c2pFd[0]);
		close(c2pFd[1]);
		param_["SCRIPT_FILENAME"] = scriptFileNameTmp;
		return h.createResponse("500");
	}
	if (pid == 0)
	{
#ifdef DEBUG
		std::stringstream msg;
		msg << "argv[0]: " << argv[0] << ", argv[1]: " << argv[1] << "\n";
		printLog(LOG_DEBUG, msg.str());
#endif
		// child process
		close(p2cFd[1]);
		dup2(p2cFd[0], STDIN_FILENO);
		close(c2pFd[0]);
		dup2(c2pFd[1], STDOUT_FILENO);
		char** pEnvrion = environ;
		chdir("/home/kisobe/.brew/bin");
		execve(pathname.c_str(), argv, pEnvrion);
		std::cerr << "execve: " << strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}
	else
	{
		close(p2cFd[0]);
		close(c2pFd[1]);
		param_["SCRIPT_FILENAME"] = scriptFileNameTmp;
		if (param_.find("HTTP_COOKIE") != param_.end())
			param_.erase("HTTP_COOKIE");
		h.setChildPid(pid);
		h.upstream = new Upstream;
		h.upstream->writeFd = p2cFd[1];
		h.upstream->readFd = c2pFd[0];
		fcntl(h.upstream->writeFd, F_SETFL, fcntl(h.upstream->writeFd, F_GETFL) | O_NONBLOCK);
		fcntl(h.upstream->readFd, F_SETFL, fcntl(h.upstream->readFd, F_GETFL) | O_NONBLOCK);
		h.upstream->p = reinterpret_cast<Protocol*>(&h);
	}
	return DONE;
}
