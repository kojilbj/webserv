#ifndef PHASEHANDLER_HPP
#define PHASEHANDLER_HPP

#include "log.hpp"

namespace Wbsv
{
	class Http;

	class PhaseHandler
	{
	public:
		virtual ~PhaseHandler() { }
		virtual int handler(Http&) = 0;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif
