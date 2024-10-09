#ifndef MODULE_HPP
#define MODULE_HPP

#include "Webserv.hpp"

namespace Webserv
{
	class Module
	{
	public:
		virtual ~Module() = 0;
		virtual init() = 0;
		virtual processEvents(Webserv::Webserv& ws) = 0;
	}
} // namespace Webserv

#endif
