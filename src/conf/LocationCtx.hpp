#ifndef LOCATIONCTX_HPP
#define LOCATIONCTX_HPP

#include <string>

namespace Wbsv
{
	class Http;

	class LocationCtx
	{
	public:
		LocationCtx()
			: limitExcept(0), autoindex(false){};
		LocationCtx(const LocationCtx& other)
			: path(other.path), limitExcept(other.limitExcept), autoindex(other.autoindex)
		{ }
		LocationCtx& operator=(const LocationCtx& other)
		{
			path = other.path;
			limitExcept = other.limitExcept;
			autoindex = other.autoindex;
			return *this;
		}
		virtual ~LocationCtx(){};
		virtual int contentHandler(Http&) = 0;
		std::string path;
		int limitExcept;
		bool autoindex;
	};
} // namespace Wbsv

#endif