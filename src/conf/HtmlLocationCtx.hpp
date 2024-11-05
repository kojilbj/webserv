#ifndef HTMLLOCATIONCTX_HPP
#define HTMLLOCATIONCTX_HPP

#include "LocationCtx.hpp"

namespace Wbsv
{
	class Http;

	class HtmlLocationCtx : public LocationCtx
	{
	public:
		HtmlLocationCtx()
			: LocationCtx()
		{ }
		HtmlLocationCtx(const HtmlLocationCtx& other)
			: LocationCtx(other), index(other.index), root(other.root)
		{ }
		HtmlLocationCtx& operator=(const HtmlLocationCtx& other)
		{
			LocationCtx::operator=(other);
			index = other.index;
			root = other.root;
			return *this;
		}
		~HtmlLocationCtx(){};
		int contentHandler(Http&);
		std::string index;
		std::string root;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif