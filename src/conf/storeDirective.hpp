#ifndef SETDIRECTIVE_HPP
#define SETDIRECTIVE_HPP

#include "CgiLocationCtx.hpp"
#include "ConfParseUtil.hpp"
#include "HtmlLocationCtx.hpp"
#include "HttpConfCtx.hpp"
#include "ReturnLocationCtx.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

//Location
void storePath(struct ConfParseUtil::SLocation& location, const std::string& value);
void storePath(struct ConfParseUtil::SLocation& location, const std::string& value);
void storeIndex(struct ConfParseUtil::SLocation& location, const std::string& value);
void storeLocationDirective(struct ConfParseUtil::SLocation& location,
							const std::string& directiveValue);
void storeCgiParam(struct ConfParseUtil::SLocation& location, const std::string& value);
void storeCgiIndex(struct ConfParseUtil::SLocation& location, const std::string& value);
void storeCgiStore(struct ConfParseUtil::SLocation& location, const std::string& value);
void storeReturn(struct ConfParseUtil::SLocation& location, const std::string& value);

//Server
void storeListen(struct ConfParseUtil::SServer&, const std::string& value);
void storeServerName(struct ConfParseUtil::SServer&, const std::string& value);
void storeClientMaxBodySize(struct ConfParseUtil::SServer&, const std::string& value);
void storeErrorPage(struct ConfParseUtil::SServer&, const std::string& value);
void storeServerDirective(struct ConfParseUtil::SServer& serverInfo,
						  const std::string& directiveValue);

#endif