#ifndef VSERVERCTX_HPP
#define VSERVERCTX_HPP

#include <iostream>
#include <string>
#include <vector>

#include "Location.hpp"

class VServerCtx
{
public:
	VServerCtx(void);
	~VServerCtx(void);

	void addLocation(void);
	void setListenPort(int listenPort);
	void setListenIP(const std::string& listenIP);
	void addServerName(const std::string& serverName);
	void setClientMaxBodySize(int clientMaxBodySize);
	std::vector<Location>& getLocations(void);

private:
	std::vector<Location> locations_;
	int listenPort_;
	std::string listenIP_;
	std::vector<std::string> serverNames_;
	int clientMaxBodySize_;
};

#endif
