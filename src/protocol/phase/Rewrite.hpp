#ifndef REWRITE_HPP
#define REWRITE_HPP

#include "PhaseHandler.hpp"

namespace Wbsv
{
	class Rewrite : public PhaseHandler
	{
	public:
		int checker(Http&);
		int handler(Http&);
	};
} // namespace Wbsv

#endif
