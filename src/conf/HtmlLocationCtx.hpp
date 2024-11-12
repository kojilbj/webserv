#ifndef HTMLLOCATIONCTX_HPP
#define HTMLLOCATIONCTX_HPP

#include "../protocol/Http.hpp"
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
		{
			index_ = "index.html";
			root_ = "html";
		}
		HtmlLocationCtx(const HtmlLocationCtx& other)
			: LocationCtx(other), index_(other.index_), root_(other.root_)
		{ }
		HtmlLocationCtx& operator=(const HtmlLocationCtx& other)
		{
			LocationCtx::operator=(other);
			index_ = other.index_;
			root_ = other.root_;
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

#endif