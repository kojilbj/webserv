#ifndef PHASEHANDLER_HPP
#define PHASEHANDLER_HPP

namespace Wbsv
{
	class Http;

	class PhaseHandler
	{
	public:
		virtual int checker(Http& http) = 0;
		virtual void handler(Http& http) = 0;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif
