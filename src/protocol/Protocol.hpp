#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include "Connection.hpp"
#include <vector>

namespace Wbsv
{
	class Connection;
	class Listening;
	class ConfCtx;
	class Protocol;

	typedef int (Protocol::*revHandler_pt)(Connection&);

	class Protocol
	{
	public:
		virtual ~Protocol(){};
		virtual void getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls) = 0;
		int invokeRevHandler(Connection& c)
		{
			return (this->*(revHandler))(c);
		}
		revHandler_pt revHandler;
		const std::string type;
		Connection c;
	};

} // namespace Wbsv

#include "ConfFile.hpp"
#include "Listening.hpp"

#endif
