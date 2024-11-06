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
		int contentHandler(Http& h);
		std::string index;
		std::map<std::string, std::string> param;
		std::string store;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif
