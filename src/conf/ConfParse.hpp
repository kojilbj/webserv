#ifndef CONFPARSE_HPP
#define CONFPARSE_HPP

#include "ConfCtx.hpp"
#include "ConfExceptions.hpp"
#include "ConfParseUtil.hpp"
#include "HttpConfCtx.hpp"
#include "storeDirective.hpp"

#include <map>
#include <stack>
#include <string>
#include <vector>

namespace Wbsv
{
	class ConfParse
	{
	public:
		ConfParse(void);
		~ConfParse(void);

		static std::vector<ConfCtx*> confParse(const std::string& confFileName);

	private:
		static std::vector<std::string> confTokenizer(std::ifstream& confFile);

		static std::map<std::string, std::vector<std::string> > mapConfRelative(void);

		static bool
		inspectStructure(const std::string& name,
						 const std::string& parentBlock,
						 std::map<std::string, std::vector<std::string> > confRelatives);

		static std::string keyValue(std::vector<std::string>::const_iterator it,
									const std::vector<std::string>& tokens);

		static ConfCtx* createCtx(const std::string& ctxName);

		static void store2Server(HttpConfCtx& httpCtx, const ConfParseUtil::SServer& serverInfo);

		static std::vector<ConfCtx*>
		parser(const std::vector<std::string>& tokens,
			   const std::map<std::string, std::vector<std::string> > confRelatives);
	};
}; // namespace Wbsv
#endif
