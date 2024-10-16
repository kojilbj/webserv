#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include "Connection.hpp"
#include <vector>

namespace Wbsv
{
	class Connection;
	class Listening;
	class ConfCtx;

	class Protocol
	{
	public:
		virtual ~Protocol(){};
		virtual void revHandler() = 0;
		virtual void getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls) = 0;
		const std::string type;
		Connection c;
	};
} // namespace Wbsv

#include "ConfFile.hpp"
#include "Listening.hpp"

#endif
