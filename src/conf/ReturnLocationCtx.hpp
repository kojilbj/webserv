#ifndef RETURNLOCATIONCTX_HPP
#define RETURNLOCATIONCTX_HPP

#include "../protocol/Http.hpp"
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
			: LocationCtx(other)
			, redirect_(other.redirect_)
			, redirectCodeDict_(other.redirectCodeDict_)
		{ }
		ReturnLocationCtx& operator=(const ReturnLocationCtx& other)
		{
			LocationCtx::operator=(other);
			redirect_ = other.redirect_;
			redirectCodeDict_ = other.redirectCodeDict_;
			return *this;
		}
		~ReturnLocationCtx() { };
		int contentHandler(Http&);
		void setRedirect(const std::string& redirect);
		const std::vector<std::string>& getRedirect(void) const;

	private:
		std::vector<std::string> redirect_;
		std::map<std::string, std::string> redirectCodeDict_;
	};
} // namespace Wbsv

#endif