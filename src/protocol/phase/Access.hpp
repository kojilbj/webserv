#ifndef ACCESS_HPP
#define ACCESS_HPP

#include "PhaseHandler.hpp"

namespace Wbsv
{
	class Access : public PhaseHandler
	{
	public:
		int handler(Http& h);
	};
} // namespace Wbsv

#endif
