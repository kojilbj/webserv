#include "storeDirective.hpp"

using namespace Wbsv;

void storePath(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	if (!location.path.empty())
		throw DuplicatedDirectiveException("Duplicated path: " + value);
	location.path = value;
}

void storeRoot(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	if (!location.root.empty())
		throw DuplicatedDirectiveException("Duplicated root: " + value);
	location.root = value;
}

void storeIndex(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	if (!location.index.empty())
		throw DuplicatedDirectiveException("Duplicated index: " + value);
	location.index = value;
}

void storeAutoIndex(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	if (!location.autoIndex.empty())
		throw DuplicatedDirectiveException("Duplicated auto_index: " + value);
	location.autoIndex = value;
}

void storeLimitExcept(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	if (!location.limitExcept.empty())
		throw DuplicatedDirectiveException("Duplicated limit_except: " + value);
	location.limitExcept = value;
}

void storeCgiIndex(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	if (!location.cgiIndex.empty())
		throw DuplicatedDirectiveException("Duplicated cgi_index: " + value);
	location.cgiIndex = value;
}

void storeCgiParam(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	if (!location.cgiParam.empty())
		throw DuplicatedDirectiveException("Duplicated cgi_param: " + value);
	location.cgiParam = value;
}

void storeCgiStore(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	if (!location.cgiStore.empty())
		throw DuplicatedDirectiveException("Duplicated cgi_store: " + value);
	location.cgiStore = value;
}

void storeReturn(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	if (!location.redirect.empty())
		throw DuplicatedDirectiveException("Duplicated retuen: " + value);
	location.redirect = value;
}

void storeLocationDirective(struct ConfParseUtil::SLocation& location,
							const std::string& directiveValue)
{
	std::string key;
	std::string value;
	std::vector<std::string> strs;

	strs = ConfParseUtil::split(directiveValue, ' ');
	if (strs.size() < 2)
		throw std::runtime_error("Empty value: " + strs[0]);
	key = strs[0];
	//listen ;のときにsegv
	for (std::vector<std::string>::const_iterator it = strs.begin() + 1; it != strs.end(); it++)
	{
		value += *it;
		if (it + 1 != strs.end())
			value += " ";
	}
	if (key == "path")
		storePath(location, value);
	else if (key == "root")
		storeRoot(location, value);
	else if (key == "index")
		storeIndex(location, value);
	else if (key == "autoindex")
		storeAutoIndex(location, value);
	else if (key == "limit_except")
		storeLimitExcept(location, value);
	else if (key == "cgi_index")
		storeCgiIndex(location, value);
	else if (key == "cgi_param")
		storeCgiParam(location, value);
	else if (key == "cgi_store")
		storeCgiStore(location, value);
	else if (key == "return")
		storeReturn(location, value);
}
