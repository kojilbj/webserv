#ifndef ERRORPAGES_HPP
#define ERRORPAGES_HPP

#include <iostream>
#include <string>
#include <vector>

namespace ConfParseUtil
{
	bool isValidErrorNumber(const std::string& errorNumber);
	bool isValidErrorNumber(int errorNumber);
} // namespace ConfParseUtil

class ErrorPage
{
public:
	ErrorPage(void);
	ErrorPage(const std::string& pagePath, const std::string& errorNumber);
	~ErrorPage(void);
	ErrorPage(const ErrorPage& other);

	void addErrorNumber(const std::string& errorNumber);
	void setErrorPagePath(const std::string& errorPagePath);
	const std::string& getErrorPagePath(void) const;
	const std::vector<std::string>& getErrorNumber(void) const;

	ErrorPage& operator=(const ErrorPage& other);

private:
	std::string errorPagePath_;
	std::vector<std::string> errorNumbers_;
};

class ErrorPages
{
public:
	ErrorPages(void);
	ErrorPages(const ErrorPages& errorPages);
	~ErrorPages(void);
	void addErrorPage(const std::string& path, const std::string& errorNumber);
	void addErrorPage(const ErrorPage& errorPage);

	const std::string& getErrorPagePath(const std::string& errorNumber) const;
	const std::string& getErrorPagePath(int errorNumber) const;

	const std::vector<ErrorPage>& getErrorPages(void) const;

	ErrorPages& operator=(const ErrorPages& other);

private:
	std::vector<ErrorPage> errorPages_;
};

#endif