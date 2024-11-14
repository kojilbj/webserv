#ifndef CONFCTX_HPP
#define CONFCTX_HPP

#include <string>
#include <vector>

namespace Wbsv
{
	class Webserv;
	class Listening;

	class ConfCtx
	{
	public:
		ConfCtx(const std::string& protoName)
			: name(protoName){};
		virtual ~ConfCtx(){};
		virtual std::string getName()
		{
			return name;
		}
		virtual void initListening(std::vector<Listening>*) const = 0;

	protected:
		std::string name;
	};

	void confParse(Webserv& ws);
} // namespace Wbsv

#include "Listening.hpp"
#include "Webserv.hpp"

#endif
