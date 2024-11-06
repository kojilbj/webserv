#ifndef VSERVERCTX_HPP
#define VSERVERCTX_HPP

#include "CgiLocationCtx.hpp"
#include "HtmlLocationCtx.hpp"
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
		VServerCtx()
			: defaultServer(false), clientMaxBodySize(0){};
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
		bool defaultServer;
		std::string serverName;
		size_t clientMaxBodySize;
		std::map<std::string, std::string> errorPages;

	private:
		std::vector<LocationCtx*> locationCtxs_;
	};
} // namespace Wbsv

#include "LocationCtx.hpp"

#endif