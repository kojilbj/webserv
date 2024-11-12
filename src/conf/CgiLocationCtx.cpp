#include "CgiLocationCtx.hpp"

CgiLocationCtx::CgiLocationCtx(void)
{
#ifdef DEBUG
	std::cout << "CgiLocationCtx Constructor Called" << std::endl;
#endif
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
void CgiLocationCtx::setCgiParam() { }

const std::map<std::string, std::string>& CgiLocationCtx::getCgiParam(void) const
{
	return cgiParam_;
}

void CgiLocationCtx::contentHandler(void) { }