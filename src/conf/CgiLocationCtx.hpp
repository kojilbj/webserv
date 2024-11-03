#ifndef CGILOCATIONCTX_HPP
#define CGILOCATIONCTX_HPP

#include "LocationCtx.hpp"
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
			: LocationCtx(other), index(other.index), param(other.param)
		{ }
		CgiLocationCtx& operator=(const CgiLocationCtx& other)
		{
			LocationCtx::operator=(other);
			index = other.index;
			param = other.param;
			return *this;
		}
		~CgiLocationCtx(){};
		int contentHandler(Http& h);
		std::string index;
		std::map<std::string, std::string> param;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif