#ifndef VSERVERCTX_HPP
#define VSERVERCTX_HPP

#include <algorithm>
#include <cctype>
#include <iostream>

#include "CgiLocationCtx.hpp"
#include "ConfParseUtil.hpp"
#include "ErrorPages.hpp"
#include "HtmlLocationCtx.hpp"
#include "LocationCtx.hpp"
#include "ReturnLocationCtx.hpp"
#include <string>
#include <vector>

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
		VServerCtx(const VServerCtx& other);
		~VServerCtx(void);

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

		VServerCtx();
		VServerCtx(const VServerCtx& other)
			: defaultServer(other.defaultServer)
			, serverName(other.serverName)
			, clientMaxBodySize(other.clientMaxBodySize)
			, errorPages(other.errorPages)
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
		~VServerCtx()
		{
			std::vector<LocationCtx*>::iterator it;
			for (it = locationCtxs_.begin(); it != locationCtxs_.end(); it++)
			{
				delete *it;
			}
		};
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