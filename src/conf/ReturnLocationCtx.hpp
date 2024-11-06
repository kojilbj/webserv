#ifndef RETURNLOCATIONCTX_HPP
#define RETURNLOCATIONCTX_HPP

#include "LocationCtx.hpp"
#include <map>

namespace Wbsv
{
	class Http;

	class ReturnLocationCtx : public LocationCtx
	{
	public:
		ReturnLocationCtx();
		ReturnLocationCtx(const ReturnLocationCtx& other)
			: LocationCtx(other), redirect(other.redirect), redirectCodeDict(other.redirectCodeDict)
		{ }
		ReturnLocationCtx& operator=(const ReturnLocationCtx& other)
		{
			LocationCtx::operator=(other);
			redirect = other.redirect;
			redirectCodeDict = other.redirectCodeDict;
			return *this;
		}
		~ReturnLocationCtx(){};
		int contentHandler(Http&);
		std::pair<std::string, std::string> redirect;
		std::map<std::string, std::string> redirectCodeDict;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif