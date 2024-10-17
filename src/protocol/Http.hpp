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
	class ServerCtx;
	class Protocol;

	class Http : public Protocol
	{
	public:
		Http()
			: serverCtx(NULL){};
		~Http(){};
		void revHandler();
		void getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls);

	private:
		ServerCtx* serverCtx;
		void waitRequestHandler();
		void wevHandler();
	};

} // namespace Wbsv

#endif
