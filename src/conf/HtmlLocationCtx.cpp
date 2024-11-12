#include "HtmlLocationCtx.hpp"

HtmlLocationCtx::HtmlLocationCtx(void)
{
#ifdef DEBUG
	std::cout << "HtmlLocationCtx Construtor Called" << std::endl;
#endif
}

HtmlLocationCtx::~HtmlLocationCtx(void) { }

void HtmlLocationCtx::setIndex(const std::string& index)
{
	index_ = index;
}

void HtmlLocationCtx::setRoot(const std::string& root)
{
	root_ = root;
}

const std::string& HtmlLocationCtx::getIndex(void) const
{
	return index_;
}
const std::string& HtmlLocationCtx::getRoot(void) const
{
	return root_;
}
void HtmlLocationCtx::contentHandler(void) { }