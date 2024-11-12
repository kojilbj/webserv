#include "ReturnLocationCtx.hpp"

ReturnLocationCtx::ReturnLocationCtx(void)
{
#ifdef DEBUG
	std::cout << "ReturnLocationCtx Constructor Called" << std::endl;
#endif
}

ReturnLocationCtx::~ReturnLocationCtx(void) { }

void ReturnLocationCtx::setReturn(const std::string& _return)
{
	std::vector<std::string> strs;

	strs = ConfParseUtil::split(_return, ' ');
	for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
	{
		if (it == strs.begin())
		{
			if (ConfParseUtil::isStatusCode(*it) == false || ConfParseUtil::isInfoCode(*it))
				throw std::logic_error("Error Invalid Return: " + _return);
			else
				return_.clear();
		}
		return_.push_back(*it);
	}
}

const std::vector<std::string>& ReturnLocationCtx::getReturn(void) const
{
	return return_;
}

void ReturnLocationCtx::contentHandler(void) { }
