#include "ErrorPages.hpp"

ErrorPage::ErrorPage(void) { }
ErrorPage::ErrorPage(const std::string& pagePath, const std::string& errorNumber)
{
	setErrorPagePath(pagePath);
	addErrorNumber(errorNumber);
}

ErrorPage::ErrorPage(const ErrorPage& other)
	: errorPagePath_(other.errorPagePath_)
{
	this->errorNumbers_.clear();
	for (std::vector<std::string>::const_iterator it = other.errorNumbers_.begin();
		 it != other.errorNumbers_.end();
		 it++)
	{
		this->errorNumbers_.push_back(*it);
	}
};

ErrorPage& ErrorPage::operator=(const ErrorPage& other)
{
	if (&other != this)
	{
		this->errorNumbers_.clear();
		for (std::vector<std::string>::const_iterator it = other.errorNumbers_.begin();
			 it != other.errorNumbers_.end();
			 it++)
		{
			this->errorNumbers_.push_back(*it);
		}
		this->errorPagePath_ = other.errorPagePath_;
	}
	return *this;
}

ErrorPage::~ErrorPage(void) { }

const std::string& ErrorPage::getErrorPagePath(void) const
{
	return errorPagePath_;
}

const std::vector<std::string>& ErrorPage::getErrorNumber(void) const
{
	return errorNumbers_;
}

void ErrorPage::addErrorNumber(const std::string& errorNumber)
{
	if (ConfParseUtil::isValidErrorNumber(errorNumber) == false)
		throw std::logic_error("Error Invalid Error Number: " + errorNumber);
	errorNumbers_.push_back(errorNumber);
}

void ErrorPage::setErrorPagePath(const std::string& errorPagePath)
{
	errorPagePath_ = errorPagePath;
}

//---------------------

ErrorPages::ErrorPages(void) { }

ErrorPages::ErrorPages(const ErrorPages& other)
{
	if (this != &other)
	{
		this->errorPages_.clear();
		for (std::vector<ErrorPage>::const_iterator it = other.errorPages_.begin();
			 it != other.errorPages_.end();
			 it++)
		{
			this->errorPages_.push_back(*it);
		}
	}
}

ErrorPages::~ErrorPages(void) { }

void ErrorPages::clear(void)
{
	if (!errorPages_.empty())
		errorPages_.clear();
}

void ErrorPages::addErrorPage(const std::string& pagePath, const std::string& errorNumber)
{
	for (std::vector<ErrorPage>::iterator it = errorPages_.begin(); it != errorPages_.end(); it++)
	{
		if ((*it).getErrorPagePath() == pagePath)
		{
			(*it).addErrorNumber(errorNumber);
			return;
		}
	}
	errorPages_.push_back(ErrorPage(pagePath, errorNumber));
}

void ErrorPages::addErrorPage(const ErrorPage& errorPage)
{
	for (std::vector<ErrorPage>::iterator it = errorPages_.begin(); it != errorPages_.end(); it++)
	{
		if ((*it).getErrorPagePath() == errorPage.getErrorPagePath())
		{
			for (std::vector<std::string>::const_iterator errorNumIt =
					 errorPage.getErrorNumber().begin();
				 errorNumIt != errorPage.getErrorNumber().end();
				 errorNumIt++)
			{
				(*it).addErrorNumber(*errorNumIt);
			}
			return;
		}
	}
	errorPages_.push_back(errorPage);
}

const std::vector<ErrorPage>& ErrorPages::getErrorPages(void) const
{
	return errorPages_;
}

std::string ErrorPages::getErrorPagePath(const std::string& errorNumber) const
{
	for (std::vector<ErrorPage>::const_iterator it = errorPages_.begin(); it != errorPages_.end();
		 it++)
	{
		for (std::vector<std::string>::const_iterator ite = (*it).getErrorNumber().begin();
			 ite != (*it).getErrorNumber().end();
			 ite++)
		{
			if ((*ite) == errorNumber)
				return (*it).getErrorPagePath();
		}
	}
	return "";
}

std::string ErrorPages::getErrorPagePath(int errorNumber) const
{
	for (std::vector<ErrorPage>::const_iterator it = errorPages_.begin(); it != errorPages_.end();
		 it++)
	{
		for (std::vector<std::string>::const_iterator ite = (*it).getErrorNumber().begin();
			 ite != (*it).getErrorNumber().end();
			 ite++)
		{
			if (std::atoi((*ite).c_str()) == errorNumber)
				return (*it).getErrorPagePath();
		}
	}
	return "";
}

ErrorPages& ErrorPages::operator=(const ErrorPages& other)
{
	if (&other != this)
	{
		errorPages_.clear();
		for (std::vector<ErrorPage>::const_iterator it = other.errorPages_.begin();
			 it != other.errorPages_.end();
			 it++)
		{
			this->errorPages_.push_back(*it);
		}
	}
	return *this;
}