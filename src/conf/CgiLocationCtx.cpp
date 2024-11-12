#include "CgiLocationCtx.hpp"

CgiLocationCtx::CgiLocationCtx(void)
{
#ifdef DEBUG
	std::cout << "CgiLocationCtx Constructor Called" << std::endl;
#endif
	cgiIndex_ = "index.php";
	store_ = "/test";
}

CgiLocationCtx::~CgiLocationCtx(void) { }

void CgiLocationCtx::setCgiIndex(const std::string& cgiIndex)
{
	if (cgiIndex.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Cgi Index: " + cgiIndex);
	cgiIndex_ = cgiIndex;
}

const std::string& CgiLocationCtx::getCgiIndex(void) const
{
	return cgiIndex_;
}
void CgiLocationCtx::setCgiParam(const std::string& key, const std::string& path)
{
	if (key.empty() || key.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Cgi Param Key: " + key);
	if (path.empty() || path.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Cgi Param Path: " + path);
	cgiParam_[key] = path;
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

const std::map<std::string, std::string>& CgiLocationCtx::getCgiParam(void) const
{
	return cgiParam_;
}

void CgiLocationCtx::contentHandler(void) { }