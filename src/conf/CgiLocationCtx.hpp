#ifndef CGILOCATIONCTX_HPP
#define CGILOCATIONCTX_HPP

#include "LocationCtx.hpp"
#include <cstdio>
#include <cstring>
#include <map>
#include <unistd.h>

namespace Wbsv
{
	class Http;

	class CgiLocationCtx : public LocationCtx
	{
	public:
		CgiLocationCtx();
		CgiLocationCtx(const CgiLocationCtx& other);
		CgiLocationCtx& operator=(const CgiLocationCtx& other);
		~CgiLocationCtx();

		void setCgiIndex(const std::string& cgiIndex);
		void setStore(const std::string& path);
		void setCgiParam(const std::string& key, const std::string& path);
		const std::string& getCgiIndex(void) const;
		const std::map<std::string, std::string>& getCgiParam(void) const;
		const std::string& getStore(void) const;
		int contentHandler(Http& h);

	private:
		std::string index_;
		std::map<std::string, std::string> param_;
		std::string store_;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif