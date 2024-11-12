#ifndef CGILOCATIONCTX_HPP
#define CGILOCATIONCTX_HPP

#include "LocationCtx.hpp"
#include <cstdio>
#include <cstring>
#include <map>
#include <unistd.h>

namespace Wbsv
{
	class Http;

	class CgiLocationCtx : public LocationCtx
	{
	public:
		CgiLocationCtx()
			: LocationCtx()
		{ }
		CgiLocationCtx(const CgiLocationCtx& other)
			: LocationCtx(other), index(other.index), param(other.param), store(other.store)
		{ }
		CgiLocationCtx& operator=(const CgiLocationCtx& other)
		{
			LocationCtx::operator=(other);
			index = other.index;
			param = other.param;
			store = other.store;
			return *this;
		}
		~CgiLocationCtx(){};

		void setCgiIndex(const std::string& cgiIndex);
		void setCgiParam(void);
		const std::string& getCgiIndex(void) const;
		const std::map<std::string, std::string>& getCgiParam(void) const;
		int contentHandler(Http& h);

	private:
		std::string index_;
		std::map<std::string, std::string> param_;
		std::string store_;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif