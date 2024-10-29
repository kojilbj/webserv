#include "VServerCtx.hpp"

VServerCtx::VServerCtx(void)
{
	std::cout << "VServerCtx Consturcor Called" << std::endl;
	listenPort_ = 80;
	listenIP_ = "*";
	clientMaxBodySize_ = 1;
	addServerName("_");
}

VServerCtx::~VServerCtx(void) { }

void VServerCtx::addLocation(void)
{
	std::cout << "VServerCtx addlocation Called" << std::endl;
	locations_.push_back(Location());
}

void VServerCtx::setListenPort(int listenPort)
{
	//エラー処理が必要
	std::cout << "VServerCtx setlistenPort Called" << std::endl;
	listenPort_ = listenPort;
}

void VServerCtx::setListenIP(const std::string& listenIP)
{
	//エラー処理が必要
	std::cout << "VServerCtx setlistenIP Called" << std::endl;
	listenIP_ = listenIP;
}

void VServerCtx::addServerName(const std::string& serverName)
{
	//エラー処理が必要
	std::cout << "VServerCtx addServerName Called" << std::endl;
	serverNames_.push_back(serverName);
}

std::vector<Location>& VServerCtx::getLocations(void)
{
	// std::cout << "VServerCtx getlocation Called" << std::endl;
	return locations_;
}

void VServerCtx::setClientMaxBodySize(int clienMacBodySize)
{
	std::cout << "VServerCtx setClientBodySize Called" << std::endl;
}

void	VServerCtx::addErrorPage(int errorNumber, const std::string & path)
{
	std::cout << "VServerCtx addErrorPage Called" << std::endl;
	std::pair<int, std::string> errorPage;

	errorPage.first = errorNumber;
	errorPage.second = path;
	errorPage_.push_back(errorPage);
}

std::vector<std::pair<int, std::string> > VServerCtx::getErrorPage(void)
{
	std::cout << "VServerCtx getErrorPage Called" << std::endl;
	return errorPage_;
}