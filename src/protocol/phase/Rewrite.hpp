#ifndef REWRITE_HPP
#define REWRITE_HPP

#include "PhaseHandler.hpp"

namespace Wbsv
{
	class Rewrite : public PhaseHandler
	{
	public:
		int handler(Http&);
	};
} // namespace Wbsv

#include "ReturnLocationCtx.hpp"

#endif
