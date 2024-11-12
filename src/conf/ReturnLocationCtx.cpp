#include "ReturnLocationCtx.hpp"

using namespace Wbsv;

ReturnLocationCtx::ReturnLocationCtx()
	: LocationCtx()
{
	redirectCodeDict_["301"] = "Moved Parmanently";
	redirectCodeDict_["302"] = "Moved Temporarily";
	redirectCodeDict_["303"] = "See Other";
	redirectCodeDict_["307"] = "Temporary Redirect";
}

int ReturnLocationCtx::contentHandler(Http& h)
{
	h.statusLine += "HTTP/1.1 " + redirect_[0] + " " + redirectCodeDict_[redirect_[0]] + "\r\n";
	if (redirect_[0] == "301" || redirect_[0] == "302" || redirect_[0] == "303" ||
		redirect_[0] == "307")
		h.headerOut += "Location: " + redirect_[1] + "\r\n";
	h.headerOut += "\r\n";
	h.messageBodyOut += redirect_[1];
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