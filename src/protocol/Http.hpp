#ifndef HTTP_HPP
#define HTTP_HPP

#include "ConfFile.hpp"
#include "Connection.hpp"
#include "HttpConfFile.hpp"
#include "Listening.hpp"
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
		void getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls);

	private:
		ServerCtx* serverCtx;
		void wevHandler(Connection& c);
	};

} // namespace Wbsv

#endif
