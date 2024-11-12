#include "VServerCtx.hpp"

VServerCtx::VServerCtx(void)
{
#ifdef DEBUG
	std::cout << "VServerCtx Consturcor Called" << std::endl;
#endif
	setClientMaxBodySize("1m");
	addServerName("_");
}

VServerCtx::VServerCtx(const VServerCtx& other)
	: defaultServer_(other.defaultServer_)
	, errorPages_(other.errorPages_)
	, clientMaxBodySize_(other.clientMaxBodySize_)
{
	this->locations_ = other.locations_;
	this->serverNames_ = other.serverNames_;
}

VServerCtx::~VServerCtx(void) { }

void VServerCtx::addLocation(LocationCtx* location)
{
#ifdef DEBUG
	std::cout << "VServerCtx addlocation Called" << std::endl;
#endif
	if (location != nullptr)
		locations_.push_back(location);
}

void VServerCtx::addLocation(std::vector<LocationCtx*> location)
{
#ifdef DEBUG
	std::cout << "VServerCtx addlocation Called" << std::endl;
#endif
	for (std::vector<LocationCtx*>::const_iterator it = location.begin(); it != location.end();
		 it++)
		locations_.push_back(*it);
}

//不必要な情報があった場合のエラーなどもここで行うのがいいと思う
void VServerCtx::addLocation(struct ConfParseUtil::SLocation location)
{
	HtmlLocationCtx* htmlLocationCtx;
	CgiLocationCtx* cgiLocationCtx;
	ReturnLocationCtx* returnLocationCtx;

	if (!location.cgiIndex.empty())
	{
		cgiLocationCtx = new CgiLocationCtx();
		if (!location.path.empty())
			cgiLocationCtx->setPath(location.path);
		if (!location.autoIndex.empty())
			cgiLocationCtx->setAutoIndex(location.autoIndex);
		if (!location.limitExcept.empty())
		{
			std::vector<std::string> strs = ConfParseUtil::split(location.limitExcept, ' ');
			for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
			{
				cgiLocationCtx->addLimitExcept(*it);
			}
		}
		if (!location.cgiIndex.empty())
			cgiLocationCtx->setCgiIndex(location.cgiIndex);
		//paramがちょい不明
		if (!location.cgiParam.empty())
			cgiLocationCtx->setCgiParam();
		addLocation(cgiLocationCtx);
	}
	else if (!location.return_.empty())
	{
		returnLocationCtx = new ReturnLocationCtx();
		if (!location.path.empty())
			returnLocationCtx->setPath(location.path);
		if (!location.limitExcept.empty())
		{
			std::vector<std::string> strs = ConfParseUtil::split(location.limitExcept, ' ');
			for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
			{
				returnLocationCtx->addLimitExcept(*it);
			}
		}
		if (!location.limitExcept.empty())
			returnLocationCtx->setLimitExcept(location.limitExcept);
		if (!location.return_.empty())
			returnLocationCtx->setReturn(location.return_);
		addLocation(returnLocationCtx);
	}
	else
	{
		htmlLocationCtx = new HtmlLocationCtx();
		if (!location.path.empty())
			htmlLocationCtx->setPath(location.path);
		if (!location.autoIndex.empty())
			htmlLocationCtx->setAutoIndex(location.autoIndex);
		if (!location.limitExcept.empty())
		{
			std::vector<std::string> strs = ConfParseUtil::split(location.limitExcept, ' ');
			for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
			{
				htmlLocationCtx->addLimitExcept(*it);
			}
		}
		if (!location.index.empty())
			htmlLocationCtx->setIndex(location.index);
		if (!location.root.empty())
			htmlLocationCtx->setRoot(location.root);
		addLocation(htmlLocationCtx);
	}
}

void VServerCtx::addLocation(std::vector<struct ConfParseUtil::SLocation> locations)
{
	for (std::vector<struct ConfParseUtil::SLocation>::iterator it = locations.begin();
		 it != locations.end();
		 it++)
	{
		addLocation(*it);
	}
}

void VServerCtx::setServerName(const std::string& serverName)
{
	if (serverName.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Server Name: " + serverName);
	serverNames_.clear();
	if (serverName == "localhost")
	{
		serverNames_.push_back("127.0.0.1");
		return;
	}
	serverNames_.push_back(serverName);
}

void VServerCtx::addServerName(const std::string& serverName)
{
//エラー処理が必要
#ifdef DEBUG
	std::cout << "VServerCtx addServerName Called" << std::endl;
#endif
	if (serverName.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Server Name: " + serverName);
	if (serverName == "localhost")
	{
		serverNames_.push_back("127.0.0.1");
		return;
	}
	serverNames_.push_back(serverName);
}

const std::vector<LocationCtx*>& VServerCtx::getLocations(void) const
{
	// std::cout << "VServerCtx getlocation Called" << std::endl;
	return locations_;
}

void VServerCtx::setDefaultServer(bool isOn)
{
	defaultServer_ = isOn;
}

void VServerCtx::setDefaultServer(const std::string& isOn)
{
	if (isOn == "on")
		defaultServer_ = true;
	else if (isOn == "off")
		defaultServer_ = false;
	else
		throw std::logic_error("Error Invalid DefaultServer: " + isOn);
}

void VServerCtx::setClientMaxBodySize(const std::string& clientMaxBodySize)
{
#ifdef DEBUG
	std::cout << "VServerCtx setClientBodySize Called" << std::endl;
#endif
	size_t size;

	if (std::all_of(clientMaxBodySize.begin(), (clientMaxBodySize.end() - 1), ::isdigit) == false)
		throw std::logic_error("Error Invalid Client Max Body Size: " + clientMaxBodySize);
	else if (!(clientMaxBodySize.back() == 'k' || clientMaxBodySize.back() == 'm' ||
			   clientMaxBodySize.back() == 'g'))
		throw std::logic_error("Error Invalhid Client Max Body Size: " + clientMaxBodySize);
	size = std::atoi(clientMaxBodySize.c_str());
	if (size < 1)
		throw std::logic_error("Error Invalid Client Max Body Size: " + clientMaxBodySize);
	if (clientMaxBodySize.back() == 'k')
		size *= 1024;
	if (clientMaxBodySize.back() == 'm')
		size *= 1024 * 1024;
	if (clientMaxBodySize.back() == 'g')
		size *= 1024 * 1024 * 1024;
	clientMaxBodySize_ = size;
}

void VServerCtx::setClientMaxBodySize(size_t clientMaxBodySize)
{
#ifdef DEBUG
	std::cout << "VServerCtx setClientBodySize Called" << std::endl;
#endif
	std::cout << clientMaxBodySize << std::endl;
	if (clientMaxBodySize < 1)
		throw std::logic_error("Error Invalid Client Max Body Size: " +
							   ConfParseUtil::intToString(clientMaxBodySize));
	clientMaxBodySize_ = clientMaxBodySize;
}

void VServerCtx::addErrorPage(const std::string& errorNumber, const std::string& path)
{
#ifdef DEBUG
	std::cout << "VServerCtx addErrorPage Called" << std::endl;
#endif
	if (path.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Path: " + path);
	if (errorNumber.find(' ') != std::string::npos ||
		std::all_of(errorNumber.begin(), errorNumber.end(), ::isdigit) == false)
		throw std::logic_error("Error Invalid Error Number: " + errorNumber);
	errorPages_.addErrorPage(path, errorNumber);
}

void VServerCtx::addErrorPage(const ErrorPage& errorPage)
{
#ifdef DEBUG
	std::cout << "VServerCtx addErrorPage Called" << std::endl;
#endif
	errorPages_.addErrorPage(errorPage);
}

const ErrorPages& VServerCtx::getErrorPages(void) const
{
#ifdef DEBUG
	std::cout << "VServerCtx getErrorPage Called" << std::endl;
#endif
	return errorPages_;
}

bool VServerCtx::isDefaultServer(void) const
{
	return defaultServer_;
}

const std::vector<std::string>& VServerCtx::getServerNames(void) const
{
	return serverNames_;
}

size_t VServerCtx::getClientMaxBodySize(void) const
{
	return clientMaxBodySize_;
}