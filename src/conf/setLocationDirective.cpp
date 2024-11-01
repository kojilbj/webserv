#include "HttpConfCtx.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

int countSpace(const std::string& line);
std::vector<std::string> split(const std::string& str);

void parsePath(Location& location, const std::string& value)
{
	if (countSpace(value) != 0)
		throw std::logic_error("Syntax Error: " + value);
	location.setPath(value);
}

void parseRoot(Location& location, const std::string& value)
{
	if (countSpace(value) != 0)
		throw std::logic_error("Syntax Error: " + value);
	location.setRoot(value);
}

void parseIndex(Location& location, const std::string& value)
{
	if (countSpace(value) != 0)
		throw std::logic_error("Syntax Error: " + value);
	location.setIndex(value);
}

void parseAutoIndex(Location& location, const std::string& value)
{
	if (countSpace(value) != 0)
		throw std::logic_error("Syntax Error: " + value);
	if (value == "on")
		location.setAutoIndex(true);
	else if (value == "off")
		location.setAutoIndex(false);
	else
		throw std::logic_error("Syntax Error: " + value);
}

void parseLimitExcept(Location& location, const std::string& value)
{
	int auth;
	std::vector<std::string> values;

	auth = 0;
	values = split(value);
	for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); it++)
	{
		if (*it == "GET")
			auth |= GET;
		else if (*it == "POST")
			auth |= POST;
		else if (*it == "DELETE")
			auth |= DELETE;
		else
			throw std::logic_error("Syntax Error: " + value);
	}
	location.setLimitExcept(auth);
}

void setLocationDirective(Location& location, std::string directiveValue)
{
	std::string key;
	std::string value;

	key = directiveValue.substr(0, directiveValue.find(" "));
	value = directiveValue.substr(directiveValue.find(" ") + 1);
	if (key == "path")
		parsePath(location, value);
	if (key == "root")
		parseRoot(location, value);
	if (key == "index")
		parseIndex(location, value);
	if (key == "autoindex")
		parseAutoIndex(location, value);
	if (key == "limit_except")
		parseLimitExcept(location, value);
}