#include "HttpConfCtx.hpp"
#include <iostream>

HttpConfCtx::HttpConfCtx(void)
{
	std::cout << "HttpConfCtx Constructor Called" << std::endl;
	name = "http";
}

HttpConfCtx::~HttpConfCtx(void) { }

void HttpConfCtx::addVServerCtx(void)
{
	servers_.push_back(VServerCtx());
}

std::vector<VServerCtx>& HttpConfCtx::getVServerCtxs(void)
{
	return (servers_);
}