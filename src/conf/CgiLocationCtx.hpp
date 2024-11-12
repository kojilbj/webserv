#ifndef CGILOCATIONCTX_HPP
#define CGILOCATIONCTX_HPP

#include "LocationCtx.hpp"
#include <map>

class CgiLocationCtx : public LocationCtx
{
public:
	CgiLocationCtx(void);
	~CgiLocationCtx(void);

	void contentHandler(void);

	void setCgiIndex(const std::string& cgiIndex);
	void setCgiParam(const std::string& key, const std::string& value);
	void setStore(const std::string& path);

	const std::string& getCgiIndex(void) const;
	const std::string& getStore(void) const;
	const std::map<std::string, std::string>& getCgiParam(void) const;

private:
	std::string cgiIndex_;
	std::string store_;
	std::map<std::string, std::string> cgiParam_;
};

#endif