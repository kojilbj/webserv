#ifndef HTMLLOCATIONCTX_HPP
#define HTMLLOCATIONCTX_HPP

#include "LocationCtx.hpp"

class HtmlLocationCtx : public LocationCtx
{
public:
	HtmlLocationCtx(void);
	~HtmlLocationCtx(void);

	void contentHandler(void);
	void setIndex(const std::string& index);
	void setRoot(const std::string& root);

	const std::string& getIndex(void) const;
	const std::string& getRoot(void) const;

private:
	std::string index_;
	std::string root_;
};

#endif