#include "VServerCtx.hpp"

using namespace Wbsv;

VServerCtx::VServerCtx(void)
	: defaultServer_(false)
{
#ifdef DEBUG
	std::cout << "VServerCtx Consturcor Called" << std::endl;
#endif
	setClientMaxBodySize("1m");
	addServerName("_");
	setDefaultServer(false);
}
VServerCtx::VServerCtx(const VServerCtx& other)
	: defaultServer_(other.defaultServer_)
	, serverNames_(other.serverNames_)
	, errorPages_(other.errorPages_)
	, clientMaxBodySize_(other.clientMaxBodySize_)
{
	std::vector<LocationCtx*>::const_iterator it = other.locationCtxs_.begin();
	HtmlLocationCtx* hlc;
	CgiLocationCtx* clc;
	ReturnLocationCtx* rlc;
	for (; it != other.locationCtxs_.end(); it++)
	{
		if ((hlc = dynamic_cast<HtmlLocationCtx*>(*it)))
		{
			HtmlLocationCtx* tmp = new HtmlLocationCtx;
			*tmp = *hlc;
			locationCtxs_.push_back(reinterpret_cast<LocationCtx*>(tmp));
		}
		else if ((clc = dynamic_cast<CgiLocationCtx*>(*it)))
		{
			CgiLocationCtx* tmp = new CgiLocationCtx;
			*tmp = *clc;
			locationCtxs_.push_back(reinterpret_cast<LocationCtx*>(tmp));
		}
		else if ((rlc = dynamic_cast<ReturnLocationCtx*>(*it)))
		{
			ReturnLocationCtx* tmp = new ReturnLocationCtx;
			*tmp = *rlc;
			locationCtxs_.push_back(reinterpret_cast<LocationCtx*>(tmp));
		}
	}
}
VServerCtx::~VServerCtx()
{
	std::vector<LocationCtx*>::iterator it;
	for (it = locationCtxs_.begin(); it != locationCtxs_.end(); it++)
	{
		delete *it;
	}
};

void VServerCtx::addLocation(LocationCtx* location)
{
#ifdef DEBUG
	std::cout << "VServerCtx addlocation Called" << std::endl;
#endif
	if (location != NULL)
	{
		if (!locationCtxs_.empty())
		{
			for (std::vector<LocationCtx*>::const_iterator it = locationCtxs_.begin();
				 it != locationCtxs_.end();
				 it++)
			{
				if ((*it)->getPath() == location->getPath())
					throw DuplicatedLocationException("Error Duplicate Path: " +
													  location->getPath());
			}
		}
		locationCtxs_.push_back(location);
	}
	sortLocationAsc();
}

void VServerCtx::addLocation(std::vector<LocationCtx*> location)
{
#ifdef DEBUG
	std::cout << "VServerCtx addlocation Called" << std::endl;
#endif
	for (std::vector<LocationCtx*>::const_iterator it = location.begin(); it != location.end();
		 it++)
		locationCtxs_.push_back(*it);
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
			cgiLocationCtx->setLimitExcept(0);
			for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
			{
				cgiLocationCtx->addLimitExcept(*it);
			}
		}
		if (!location.cgiIndex.empty())
			cgiLocationCtx->setIndex(location.cgiIndex);
		//paramがちょい不明
		if (!location.cgiParam.empty())
		{
			std::string key = location.cgiParam.substr(0, location.cgiParam.find(' '));
			std::string path = location.cgiParam.substr(location.cgiParam.find(' ') + 1);
			cgiLocationCtx->setParam(key, path);
		}
		if (!location.cgiStore.empty())
		{
			cgiLocationCtx->setStore(location.cgiStore);
		}
		addLocation(cgiLocationCtx);
	}
	else if (!location.redirect.empty())
	{
		returnLocationCtx = new ReturnLocationCtx();
		if (!location.path.empty())
			returnLocationCtx->setPath(location.path);
		if (!location.limitExcept.empty())
		{
			returnLocationCtx->setLimitExcept(0);
			std::vector<std::string> strs = ConfParseUtil::split(location.limitExcept, ' ');
			for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
			{
				returnLocationCtx->addLimitExcept(*it);
			}
		}
		if (!location.limitExcept.empty())
			returnLocationCtx->setLimitExcept(location.limitExcept);
		if (!location.redirect.empty())
			returnLocationCtx->setRedirect(location.redirect);
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
			htmlLocationCtx->setLimitExcept(0);
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

bool VServerCtx::deeperNestAsc(const LocationCtx* l1, const LocationCtx* l2)
{
	std::vector<std::string> s1;
	std::vector<std::string> s2;

	s1 = ConfParseUtil::split(l1->getPath(), '/');
	s2 = ConfParseUtil::split(l2->getPath(), '/');
	return s1.size() < s2.size();
}

bool VServerCtx::deeperNestDsc(const LocationCtx* l1, const LocationCtx* l2)
{
	std::vector<std::string> s1;
	std::vector<std::string> s2;

	s1 = ConfParseUtil::split(l1->getPath(), '/');
	s2 = ConfParseUtil::split(l2->getPath(), '/');
	return s1.size() > s2.size();
}

void VServerCtx::sortLocationAsc(void)
{
	std::sort(locationCtxs_.begin(), locationCtxs_.end(), deeperNestAsc);
}

void VServerCtx::sortLocationDsc(void)
{
	std::sort(locationCtxs_.begin(), locationCtxs_.end(), deeperNestDsc);
}

void VServerCtx::setServerName(const std::string& serverName)
{
	if (serverName.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Server Name: " + serverName);
	serverNames_.clear();
	// if (serverName == "localhost")
	// {
	// 	serverNames_.push_back("127.0.0.1");
	// 	return;
	// }
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
	// if (serverName == "localhost")
	// {
	// 	serverNames_.push_back("127.0.0.1");
	// 	return;
	// }
	serverNames_.push_back(serverName);
}

const std::vector<LocationCtx*>& VServerCtx::getLocations(void) const
{
	// std::cout << "VServerCtx getlocation Called" << std::endl;
	return locationCtxs_;
}

void VServerCtx::setDefaultServer(bool isOn)
{
	defaultServer_ = isOn;
}

void VServerCtx::setClientMaxBodySize(const std::string& clientMaxBodySize)
{
#ifdef DEBUG
	std::cout << "VServerCtx setClientBodySize Called" << std::endl;
#endif
	size_t size;

	if (ConfParseUtil::isHeadZero(clientMaxBodySize))
		throw std::logic_error("Error Invalid Client Max Body Size: " + clientMaxBodySize);
	if (!ConfParseUtil::isAllDigit(clientMaxBodySize, clientMaxBodySize.end() - 1))
		throw std::logic_error("Error Invalhid Client Max Body Size: " + clientMaxBodySize);
	if (!std::isdigit(*clientMaxBodySize.end() - 1))
	{
		if (!(*(clientMaxBodySize.end() - 1) == 'k' || *(clientMaxBodySize.end() - 1) == 'm' ||
			  *(clientMaxBodySize.end() - 1) == 'g'))
			throw std::logic_error("Error Invalhid Client Max Body Size: " + clientMaxBodySize);
	}
	size = std::atoi(clientMaxBodySize.c_str());
	if (size < 1)
		throw std::logic_error("Error Invalid Client Max Body Size: " + clientMaxBodySize);
	if (*(clientMaxBodySize.end() - 1) == 'k')
		size *= 1024;
	if (*(clientMaxBodySize.end() - 1) == 'm')
		size *= 1024 * 1024;
	if (*(clientMaxBodySize.end() - 1) == 'g')
		size *= 1024 * 1024 * 1024;
	setClientMaxBodySize(size);
}

void VServerCtx::setClientMaxBodySize(size_t clientMaxBodySize)
{
#ifdef DEBUG
	std::cout << "VServerCtx setClientBodySize Called" << std::endl;
#endif
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
	if (errorNumber.find(' ') != std::string::npos || !ConfParseUtil::isAllDigit(errorNumber) ||
		ConfParseUtil::isHeadZero(errorNumber))
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

bool VServerCtx::hasServerName(const std::string& host) const
{
	for (std::vector<std::string>::const_iterator it = serverNames_.begin();
		 it != serverNames_.end();
		 it++)
	{
		if (*it == host)
			return true;
	}
	return false;
}