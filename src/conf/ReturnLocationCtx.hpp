#ifndef RETURNLOCATIONCTX_HPP
#define RETURNLOCATIONCTX_HPP

#include "LocationCtx.hpp"
#include <map>

class ReturnLocationCtx : public LocationCtx
{
public:
	ReturnLocationCtx(void);
	~ReturnLocationCtx(void);

	void contentHandler(void);
	void setReturn(const std::string& _return);

	const std::vector<std::string>& getReturn(void) const;

private:
	std::vector<std::string> return_;
};

#endif