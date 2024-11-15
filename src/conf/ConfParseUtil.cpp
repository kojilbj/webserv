#include "ConfParseUtil.hpp"

bool ConfParseUtil::isStatusCode(int code)
{
	if (100 <= code && code < 600)
		return true;
	else
		return false;
}

bool ConfParseUtil::isStatusCode(const std::string& code)
{
	if (!isAllDigit(code) || isHeadZero(code))
		return false;
	return isStatusCode(std::atoi(code.c_str()));
}

bool ConfParseUtil::isInfoCode(int statusCode)
{
	if (100 <= statusCode && statusCode < 200)
		return true;
	else
		return false;
}

bool ConfParseUtil::isInfoCode(const std::string& statusCode)
{
	if (isStatusCode(statusCode) == false)
		return false;
	return isInfoCode(std::atoi(statusCode.c_str()));
}

bool ConfParseUtil::isSuccessCode(int statusCode)
{
	if (200 <= statusCode && statusCode < 300)
		return true;
	else
		return false;
}

bool ConfParseUtil::isSuccessCode(const std::string& statusCode)
{
	if (isStatusCode(statusCode) == false)
		return false;
	return isSuccessCode(std::atoi(statusCode.c_str()));
}

bool ConfParseUtil::isRedirectCode(int statusCode)
{
	if (300 <= statusCode && statusCode < 400)
		return true;
	else
		return false;
}

bool ConfParseUtil::isRedirectCode(const std::string& statusCode)
{
	if (isStatusCode(statusCode) == false)
		return false;
	return isRedirectCode(std::atoi(statusCode.c_str()));
}

bool ConfParseUtil::isClientErrortCode(int statusCode)
{
	if (400 <= statusCode && statusCode < 500)
		return true;
	else
		return false;
}

bool ConfParseUtil::isClientErrortCode(const std::string& statusCode)
{
	if (isStatusCode(statusCode) == false)
		return false;
	return isClientErrortCode(std::atoi(statusCode.c_str()));
}

bool ConfParseUtil::isServerErrortCode(int statusCode)
{
	if (500 <= statusCode && statusCode < 600)
		return true;
	else
		return false;
}

bool ConfParseUtil::isServerErrortCode(const std::string& statusCode)
{
	if (isStatusCode(statusCode) == false)
		return false;
	return isServerErrortCode(std::atoi(statusCode.c_str()));
}

int ConfParseUtil::countSpace(const std::string& line)
{
	int count;
	std::string::const_iterator it;

	it = line.begin();
	count = 0;
	while (it != line.end())
	{
		if (*it == ' ')
			count++;
		it++;
	}
	return count;
}

int ConfParseUtil::countDot(const std::string& line)
{
	int count;
	std::string::const_iterator it;

	it = line.begin();
	count = 0;
	while (it != line.end())
	{
		if (*it == '.')
			count++;
		it++;
	}
	return count;
}

bool ConfParseUtil::isValidIPAddress(const std::string& ip)
{
	std::vector<std::string> strs;

	if (ip == "localhost")
		return true;
	if (countSpace(ip) != 0)
		return false;
	if (countDot(ip) != 3)
		return false;
	strs = split(ip, '.');
	for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
	{
		if (!isAllDigit(*it))
			return false;
		if (!std::atoi(it->c_str()) == 0 && ConfParseUtil::isHeadZero(ip))
			return false;
		if (std::atoi(it->c_str()) > 255)
			return false;
	}
	return true;
}

bool ConfParseUtil::isValidPort(const std::string& port)
{
	int portNumber;

	if (!isAllDigit(port) || isHeadZero(port))
		return false;
	portNumber = std::atoi(port.c_str());
	if (!(0 < portNumber && portNumber < 65536))
		return false;
	return true;
}

bool ConfParseUtil::isValidPort(int port)
{
	if (!(0 < port && port < 65536))
		return false;
	return true;
}

bool ConfParseUtil::isValidErrorNumber(const std::string& errorNumber)
{
	int number;

	if (errorNumber.find(' ') != std::string::npos)
		return false;
	if (!isAllDigit(errorNumber) || isHeadZero(errorNumber))
		return false;
	number = std::atoi(errorNumber.c_str());
	if (!(400 <= number && number < 600))
		return false;
	return true;
}

bool ConfParseUtil::isValidErrorNumber(int errorNumber)
{
	if (!(400 <= errorNumber && errorNumber < 600))
		return false;
	return true;
}

std::string ConfParseUtil::intToString(int i)
{
	std::string str;
	char num;
	bool isMinus;

	if (i == 0)
		return "0";
	isMinus = false;
	if (i < 0)
	{
		i *= -1;
		isMinus = true;
	}
	while (i != 0)
	{
		num = (i % 10) + '0';
		str = num + str;
		i /= 10;
	}
	if (isMinus)
		str = "-" + str;
	return str;
}

std::vector<std::string> ConfParseUtil::split(std::ifstream& file, char c)
{
	std::string buff;
	std::vector<std::string> tokens;

	while (std::getline(file, buff, c))
	{
		if (!buff.empty())
		{
			// std::cout << buff << std::endl;
			tokens.push_back(buff);
		}
	}
	return tokens;
}

std::vector<std::string> ConfParseUtil::split(const std::string& str, char c)
{
	std::string buff;
	std::stringstream sstr(str);
	std::vector<std::string> tokens;

	while (std::getline(sstr, buff, c))
	{
		if (!buff.empty())
		{
			// std::cout << buff << std::endl;
			tokens.push_back(buff);
		}
	}
	return tokens;
}

bool ConfParseUtil::isAllDigit(const std::string& str, std::string::const_iterator endIt)
{
	if (str.empty())
		return false;
	for (std::string::const_iterator it = str.begin(); it != endIt; it++)
	{
		if (!std::isdigit(*it))
			return false;
	}
	return true;
}

bool ConfParseUtil::isAllDigit(const std::string& str)
{
	if (str.empty())
		return false;
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++)
	{
		if (!std::isdigit(*it))
			return false;
	}
	return true;
}

bool ConfParseUtil::isHeadZero(const std::string& str)
{
	if (str.empty())
		return false;
	if (*str.begin() == '0')
		return true;
	else
		return false;
}

// size_t ConfParseUtil::toSizeT(const std::string& str)
// {
// 	size_t num;

// 	num = 0;
// 	for (std::string::const_iterator it = str.begin(); it != str.end(); it++)
// 	{
// 		if (it == str.begin() && *it == '-')
// 			;
// 	}
// }