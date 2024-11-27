#ifndef CONFPARSEUTIL_HPP
#define CONFPARSEUTIL_HPP

#include "ErrorPages.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace ConfParseUtil
{
	std::vector<std::string> split(const std::string& str, char c);
	std::vector<std::string> split(const std::string& str);
	std::vector<std::string> split(std::ifstream& file, char c);

	int count(const std::string& line, char c);

	bool isValidIPAddress(const std::string& ip);

	bool isValidPort(int port);
	bool isValidPort(const std::string& port);

	bool isValidErrorNumber(int errorNumber);
	bool isValidErrorNumber(const std::string& errorNumber);

	bool isStatusCode(int code);
	bool isStatusCode(const std::string& code);

	bool isInfoCode(int statusCode);
	bool isInfoCode(const std::string& statusCode);

	bool isSuccessCode(int statusCode);
	bool isSuccessCode(const std::string& statusCode);

	bool isRedirectCode(int statusCode);
	bool isRedirectCode(const std::string& statusCode);

	bool isClientErrortCode(int statusCode);
	bool isClientErrortCode(const std::string& statusCode);

	bool isServerErrortCode(int statusCode);
	bool isServerErrortCode(const std::string& statusCode);

	bool isAllDigit(const std::string& str, std::string::const_iterator it);
	bool isAllDigit(const std::string& str);
	bool isHeadZero(const std::string& str);

	std::string intToString(int i);
	std::string ipv4NameResolution(const std::string& hostName);

	//LocationCtxは種類があるため判別のために初期値はいれない
	struct SLocation
	{
		std::string path;
		std::string root;
		std::string index;
		std::string autoIndex;
		std::string limitExcept;

		std::string cgiIndex;
		std::string cgiParam;
		std::string cgiStore;

		std::string redirect;
	};

	//parse時にserverに関する情報をまとめておきたい。VServerとの情報の切り離しがconfのparse時は厄介だから
	struct SServer
	{
		std::string listenPort;
		std::string listenIP;
		std::string defaultServer;
		std::vector<struct SLocation> locations;
		std::vector<std::string> serverNames;
		ErrorPages errorPages;
		std::string clientMaxBodySize;
	};
} // namespace ConfParseUtil

#endif