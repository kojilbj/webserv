#include "HttpConfCtx.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

int countSpace(const std::string& line);

void parseListen(VServerCtx& vsCtx, const std::string& value)
{
	std::vector<std::string> str;
	std::string listenPort;
	std::string listenIP;

	if (countSpace(value) != 0)
		throw std::logic_error("Syntax Error: " + value);
	if (value.find(':') != std::string::npos)
	{
		listenIP = value.substr(0, value.find(":"));
		listenPort = value.substr(value.find(":") + 1);
		vsCtx.setListenIP(listenIP);
		vsCtx.setListenPort(std::atoi(listenPort.c_str()));
	}
	else
	{
		listenPort = value.substr(value.find(' ') + 1);
		vsCtx.setListenPort(std::atoi(listenPort.c_str()));
	}
}

void parseServerName(VServerCtx& vsCtx, const std::string& value)
{
	std::stringstream sstr(value);
	std::string buff;

	while (std::getline(sstr, buff, ' '))
		vsCtx.addServerName(buff);
}

void parseClientMaxBodySize(VServerCtx& vsCtx, const std::string& value)
{
	if (countSpace(value) != 0)
		throw std::logic_error("Syntax Error: " + value);
	vsCtx.setClientMaxBodySize(std::atoi(value.substr(value.find(' ') + 1).c_str()));
}

void parseErrorPage(VServerCtx& vsCtx, const std::string& value)
{
	std::string errorNum;
	std::string path;

	if (countSpace(value) != 1)
		throw std::logic_error("Syntax Error: " + value);
	errorNum = value.substr(0, value.find(" "));
	path = value.substr(value.find(" ") + 1);
	vsCtx.addErrorPage(std::atoi(errorNum.c_str()), path);
}

void setServerDirective(VServerCtx& vsCtx, const std::string& directiveValue)
{
	std::string key;
	std::string value;

	key = directiveValue.substr(0, directiveValue.find(" "));
	value = directiveValue.substr(directiveValue.find(" ") + 1);
	if (key == "listen")
		parseListen(vsCtx, value);
	if (key == "server_name")
		parseServerName(vsCtx, value);
	if (key == "client_max_body_size")
		parseClientMaxBodySize(vsCtx, value);
	if (key == "error_page")
		parseErrorPage(vsCtx, value);
}