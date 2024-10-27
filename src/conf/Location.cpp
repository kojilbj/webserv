#include "Location.hpp"

Location::Location(void)
{
	std::cout << "Location Constructor Called" << std::endl;
	root_ = "html/";
	index_ = "index.html";
	limit_except_ = GET;
	autoindex_ = false;
}

Location::~Location(void) { }
