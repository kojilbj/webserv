#include "storeDirective.hpp"

using namespace Wbsv;

void storeListen(struct ConfParseUtil::SServer& serverInfo, const std::string& value)
{
	std::vector<std::string> str;
	std::string listenPort;
	std::string listenIP;

	if (!serverInfo.listenPort.empty())
		throw DuplicatedDirectiveException("Duplicated listen: " + value);
	if (ConfParseUtil::count(value, ' ') != 0)
		throw std::logic_error("Syntax Error: " + value);
	str = ConfParseUtil::split(value, ':');
	if (str.size() > 2)
		throw std::logic_error("Syntax Error: " + value);
	else if (str.size() == 2)
	{
		serverInfo.listenIP = str[0];
		serverInfo.listenPort = str[1];
	}
	else
	{
		//ポート番号なのかIPアドレスなのか判定する必要がある
		if (ConfParseUtil::isAllDigit(str[0]))
			serverInfo.listenPort = str[0];
		else
			serverInfo.listenIP = str[0];
	}
}

void storeServerName(struct ConfParseUtil::SServer& serverInfo, const std::string& value)
{
	std::stringstream sstr(value);
	std::string buff;

	if (!serverInfo.serverNames.empty())
		throw DuplicatedDirectiveException("Duplicated server_name: " + value);
	while (std::getline(sstr, buff, ' '))
		serverInfo.serverNames.push_back(buff);
}

void storeClientMaxBodySize(struct ConfParseUtil::SServer& serverInfo, const std::string& value)
{
	if (!serverInfo.clientMaxBodySize.empty())
		throw DuplicatedDirectiveException("Duplicated client_max_body_size: " + value);
	if (ConfParseUtil::count(value, ' ') != 0)
		throw std::logic_error("Syntax Error: " + value);
	serverInfo.clientMaxBodySize = value;
}

void storeErrorPage(struct ConfParseUtil::SServer& serverInfo, const std::string& value)
{
	std::vector<std::string> strs;

	if (ConfParseUtil::count(value, ' ') != 1)
		throw std::logic_error("Syntax Error: " + value);
	strs = ConfParseUtil::split(value, ' ');
	for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end() - 1; it++)
	{
		serverInfo.errorPages.addErrorPage(*(strs.end() - 1), *it);
	}
}

void storeServerDirective(struct ConfParseUtil::SServer& serverInfo,
						  const std::string& directiveValue)
{
	std::string key;
	std::string value;
	std::vector<std::string> strs;

	strs = ConfParseUtil::split(directiveValue, ' ');
	if (strs.size() < 2)
		throw std::runtime_error("Empty value: " + strs[0]);
	key = strs[0];
	for (std::vector<std::string>::const_iterator it = strs.begin() + 1; it != strs.end(); it++)
	{
		value += *it;
		if (it + 1 != strs.end())
			value += " ";
	}
	if (key == "listen")
		storeListen(serverInfo, value);
	else if (key == "server_name")
		storeServerName(serverInfo, value);
	else if (key == "client_max_body_size")
		storeClientMaxBodySize(serverInfo, value);
	else if (key == "error_page")
		storeErrorPage(serverInfo, value);
}