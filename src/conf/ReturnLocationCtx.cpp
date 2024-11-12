#include "ReturnLocationCtx.hpp"

using namespace Wbsv;

ReturnLocationCtx::ReturnLocationCtx()
	: LocationCtx()
{
	redirectCodeDict["301"] = "Moved Parmanently";
	redirectCodeDict["302"] = "Moved Temporarily";
	redirectCodeDict["303"] = "See Other";
	redirectCodeDict["307"] = "Temporary Redirect";
}

int ReturnLocationCtx::contentHandler(Http& h)
{
	h.statusLine += "HTTP/1.1 " + redirect.first + " " + redirectCodeDict[redirect.first] + "\r\n";
	if (redirect.first == "301" || redirect.first == "302" || redirect.first == "303" ||
		redirect.first == "307")
		h.headerOut += "Location: " + redirect.second + "\r\n";
	h.headerOut += "\r\n";
	h.messageBodyOut += redirect.second;
	/* if ( rewrite destination is like "http://www.google.com" ) */
	/* redirect response */
	/* if ( rewrite destination is like "localhost:80/something" ) */
	/* internal redirect (jump to FindConfig Phase) */
	/* count redirect (redirect loop is up to 5) */
	return DONE;
}

void ReturnLocationCtx::setRedirect(const std::string& redirect)
{
	std::vector<std::string> strs;

	strs = ConfParseUtil::split(redirect, ' ');
	for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
	{
		if (it == strs.begin())
		{
			if (ConfParseUtil::isStatusCode(*it) == false || ConfParseUtil::isInfoCode(*it))
				throw std::logic_error("Error Invalid Return: " + redirect);
			else
				redirect_.clear();
		}
		redirect_.push_back(*it);
	}
}

const std::vector<std::string>& ReturnLocationCtx::getRedirect(void) const
{
	return redirect_;
}