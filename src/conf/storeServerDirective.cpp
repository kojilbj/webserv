#include "storeDirective.hpp"

void storeListen(struct ConfParseUtil::SServer& serverInfo, const std::string& value)
{
	std::vector<std::string> str;
	std::string listenPort;
	std::string listenIP;

	if (ConfParseUtil::countSpace(value) != 0)
		throw std::logic_error("Syntax Error: " + value);
	if (value.find(':') != std::string::npos)
	{
		listenIP = value.substr(0, value.find(":"));
		listenPort = value.substr(value.find(":") + 1);
		serverInfo.listenIP = listenIP;
		serverInfo.listenPort = listenPort;
	}
	else
	{
		listenPort = value.substr(value.find(' ') + 1);
		serverInfo.listenPort = listenPort;
	}
}

void storeServerName(struct ConfParseUtil::SServer& serverInfo, const std::string& value)
{
	std::stringstream sstr(value);
	std::string buff;

	while (std::getline(sstr, buff, ' '))
		serverInfo.serverNames.push_back(buff);
}

void storeClientMaxBodySize(struct ConfParseUtil::SServer& serverInfo, const std::string& value)
{
	if (ConfParseUtil::countSpace(value) != 0)
		throw std::logic_error("Syntax Error: " + value);
	serverInfo.clientMaxBodySize = value;
}

void storeErrorPage(struct ConfParseUtil::SServer& serverInfo, const std::string& value)
{
	std::string errorNum;
	std::string path;

	if (ConfParseUtil::countSpace(value) != 1)
		throw std::logic_error("Syntax Error: " + value);
	errorNum = value.substr(0, value.find(" "));
	path = value.substr(value.find(" ") + 1);
	serverInfo.errorPages.addErrorPage(path, errorNum);
}

void storeServerDirective(struct ConfParseUtil::SServer& serverInfo,
						  const std::string& directiveValue)
{
	std::string key;
	std::string value;

	key = directiveValue.substr(0, directiveValue.find(" "));
	value = directiveValue.substr(directiveValue.find(" ") + 1);
	if (key == "listen")
		storeListen(serverInfo, value);
	if (key == "server_name")
		storeServerName(serverInfo, value);
	if (key == "client_max_body_size")
		storeClientMaxBodySize(serverInfo, value);
	if (key == "error_page")
		storeErrorPage(serverInfo, value);
}