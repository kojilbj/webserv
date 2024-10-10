#ifndef HTTP_HPP
#define HTTP_HPP

namespace Webserv
{
	class Http
	{
	public:
		Http();
		~Http();

	private:
		Webserv::HttpConfCtx* confCtx;
	}

	void
	httpInitConnection(Webserv::Connection& c);

} // namespace Webserv

#endif
