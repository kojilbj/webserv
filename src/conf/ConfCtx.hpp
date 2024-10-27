#ifndef CONFCTX_HPP
#define CONFCTX_HPP

#include <iostream>
#include <string>

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
