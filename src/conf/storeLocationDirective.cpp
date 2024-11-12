#include "storeDirective.hpp"

void storePath(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	location.path = value;
}

void storeRoot(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	location.root = value;
}

void storeIndex(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	location.index = value;
}

void storeAutoIndex(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	location.autoIndex = value;
}

void storeLimitExcept(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	location.limitExcept = value;
}

void storeCgiIndex(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	location.cgiIndex = value;
}

void storeCgiParam(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	location.cgiParam = value;
}

void storeReturn(struct ConfParseUtil::SLocation& location, const std::string& value)
{
	location.return_ = value;
}

void storeLocationDirective(struct ConfParseUtil::SLocation& location,
							const std::string& directiveValue)
{
	std::string key;
	std::string value;

	key = directiveValue.substr(0, directiveValue.find(" "));
	value = directiveValue.substr(directiveValue.find(" ") + 1);
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
	else if (key == "return")
		storeReturn(location, value);
}