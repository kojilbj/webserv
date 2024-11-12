#ifndef CONFCTX_HPP
#define CONFCTX_HPP

#include "ErrorPages.hpp"
#include "LocationCtx.hpp"

#include <iostream>
#include <string>
#include <vector>

class ConfCtx
{
public:
	ConfCtx(void);
	virtual ~ConfCtx(void) = 0;

	std::string getName(void);

protected:
	std::string name;

private:
};

#endif
