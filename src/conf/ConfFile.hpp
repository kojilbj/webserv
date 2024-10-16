#ifndef CONFFILE_HPP
#define CONFFILE_HPP

#include <errno.h>
#include <string>
#include <vector>

namespace Wbsv
{
	class Webserv;
	class Listening;

	class ConfCtx
	{
	public:
		virtual ~ConfCtx(){};
		virtual std::string getProtocol() const = 0;
		virtual void initListening(std::vector<Listening>*) const = 0;
	};

	void confParse(Webserv& ws);
} // namespace Wbsv

#include "Listening.hpp"
#include "Webserv.hpp"

#endif
