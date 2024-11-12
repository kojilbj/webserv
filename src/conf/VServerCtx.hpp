#ifndef VSERVERCTX_HPP
#define VSERVERCTX_HPP

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "CgiLocationCtx.hpp"
#include "ConfParseUtil.hpp"
#include "ErrorPages.hpp"
#include "HtmlLocationCtx.hpp"
#include "LocationCtx.hpp"
#include "ReturnLocationCtx.hpp"

namespace Wbsv
{
	class HtmlLocationCtx;
	class CgiLocationCtx;
	class ReturnLocationCtx;
	class LocationCtx;

	class VServerCtx
	{
	public:
		VServerCtx(void);

		void addLocation(LocationCtx* location);
		void addLocation(const std::vector<LocationCtx*> locations);
		void addLocation(std::vector<struct ConfParseUtil::SLocation> locations);
		void addLocation(struct ConfParseUtil::SLocation locations);

		void setServerName(const std::string& serverName);
		void addServerName(const std::string& serverName);

		void setClientMaxBodySize(const std::string& clientMaxBodySize);
		void setClientMaxBodySize(size_t clientMaxBodySize);

		void addErrorPage(const std::string& errorNumber, const std::string& path);
		void addErrorPage(const ErrorPage& errorPage);

		void setDefaultServer(bool isOn);
		void setDefaultServer(const std::string& isOn);

		bool isDefaultServer(void) const;

		const ErrorPages& getErrorPages(void) const;
		size_t getClientMaxBodySize(void) const;
		const std::vector<LocationCtx*>& getLocations(void) const;
		const std::vector<std::string>& getServerNames(void) const;
		bool getDefaultServer(void) const
		{
			return defaultServer_;
		}

		VServerCtx(const VServerCtx& other);
		~VServerCtx(void);
		std::vector<LocationCtx*>* getLocationCtxs()
		{
			return &locationCtxs_;
		}
		void addLocationCtx(LocationCtx* lc)
		{
			locationCtxs_.push_back(lc);
		}

	private:
		bool defaultServer_;
		std::vector<std::string> serverNames_;
		ErrorPages errorPages_;
		size_t clientMaxBodySize_;
		std::vector<LocationCtx*> locationCtxs_;
	};
} // namespace Wbsv

#include "LocationCtx.hpp"

#endif