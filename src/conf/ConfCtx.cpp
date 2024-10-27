#include "ConfCtx.hpp"

ConfCtx::ConfCtx(void) { }

ConfCtx::~ConfCtx(void) { }

std::string ConfCtx::getName(void)
{
	std::cout << "ConfCtx getName Called" << std::endl;
	return (name);
}
