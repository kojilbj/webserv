#ifndef FINDCONFIG_HPP
#define FINDCONFIG_HPP

#include "PhaseHandler.hpp"

namespace Wbsv
{
	class PhaseHandler;
	class FindConfig : public PhaseHandler
	{
	public:
		int checker(Http& http);
		void handler(Http& http);
	};
} // namespace Wbsv

#endif
