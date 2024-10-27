#ifndef HTTPCONFCTX_HPP
#define HTTPCONFCTX_HPP

#include <vector>

#include "ConfCtx.hpp"
#include "VServerCtx.hpp"

class HttpConfCtx : public ConfCtx
{
public:
	HttpConfCtx(void);
	~HttpConfCtx(void);

	// getMainCtx(void);
	// getServerCtx(void);
	void addVServerCtx(void);
	std::vector<VServerCtx>& getVServerCtxs(void);
	// initListening(void);

private:
	std::vector<VServerCtx> servers_;
};

#endif
