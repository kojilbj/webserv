#ifndef LOCATIONCTX_HPP
#define LOCATIONCTX_HPP

#include "ConfParseUtil.hpp"
#include "Http.hpp"
#include <string>

namespace Wbsv
{
	class Http;

	class LocationCtx
	{
	public:
		LocationCtx()
			: limitExcept(GET), autoindex(false){};
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

		void setPath(const std::string& path);
		void setLimitExcept(int auth);
		void setLimitExcept(const std::string& auth);
		void addLimitExcept(int auth);
		void addLimitExcept(const std::string& auth);
		void setAutoIndex(bool isOn);
		void setAutoIndex(const std::string& autoIndex);

		const std::string& getPath(void) const;
		int getLimitExcept(void) const;
		bool allowGet(void) const;
		bool allowPost(void) const;
		bool allowDelete(void) const;
		bool isAutoIndex(void) const;

	protected:
		std::string path;
		int limitExcept;
		bool autoindex;
	};
} // namespace Wbsv

#endif