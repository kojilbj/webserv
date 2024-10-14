#ifndef HTTP_HPP
#define HTTP_HPP

#include "Connection.hpp"
#include "HttpConfFile.hpp"
#include "Protocol.hpp"
#include <iostream>
#include <unistd.h>

namespace Wbsv
{
	class HttpConfCtx;
	class Protocol;

	class Http : public Protocol
	{
	public:
		Http(){};
		~Http(){};
		void revHandler(Connection& c);

	private:
		HttpConfCtx* confCtx;
		void wevHandler(Connection& c);
	};

} // namespace Wbsv

#endif
