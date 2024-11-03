#ifndef RETURNLOCATIONCTX_HPP
#define RETURNLOCATIONCTX_HPP

#include "LocationCtx.hpp"

namespace Wbsv
{
	class Http;

	class ReturnLocationCtx : public LocationCtx
	{
	public:
		ReturnLocationCtx()
			: LocationCtx()
		{ }
		ReturnLocationCtx(const ReturnLocationCtx& other)
			: LocationCtx(other), redirect(other.redirect)
		{ }
		ReturnLocationCtx& operator=(const ReturnLocationCtx& other)
		{
			LocationCtx::operator=(other);
			redirect = other.redirect;
			return *this;
		}
		~ReturnLocationCtx(){};
		int contentHandler(Http&);
		std::pair<std::string, std::string> redirect;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif