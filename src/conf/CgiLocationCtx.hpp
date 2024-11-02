#ifndef CGILOCATIONCTX_HPP
#define CGILOCATIONCTX_HPP

#include "LocationCtx.hpp"
#include <map>

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
			: LocationCtx(other), index(other.index), root(other.root)
		{ }
		CgiLocationCtx& operator=(const CgiLocationCtx& other)
		{
			LocationCtx::operator=(other);
			index = other.index;
			root = other.root;
			return *this;
		}
		~CgiLocationCtx(){};
		int contentHandler(Http& h);
		std::string index;
		std::string root;
		// std::map<std::string, std::string> param;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif