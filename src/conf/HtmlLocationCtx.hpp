#ifndef HTMLLOCATIONCTX_HPP
#define HTMLLOCATIONCTX_HPP

#include "LocationCtx.hpp"
#include <dirent.h>
#include <sstream>
#include <sys/types.h>

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
		void setIndex(const std::string& index);
		void setRoot(const std::string& root);
		const std::string& getIndex(void) const;
		const std::string& getRoot(void) const;

	private:
		std::string index_;
		std::string root_;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif