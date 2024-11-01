#pragma once
#include "HttpConfCtx.hpp"

void parsePath(Location& location, const std::string& value);
void parsePath(Location& location, const std::string& value);
void parseIndex(Location& location, const std::string& value);
void setLocationDirective(Location& location, std::string directiveValue);
void parseListen(VServerCtx& vsCtx, const std::string& value);
void parseServerName(VServerCtx& vsCtx, const std::string& value);
void parseClientMaxBodySize(VServerCtx& vsCtx, const std::string& value);
void parseErrorPage(VServerCtx& vsCtx, const std::string& value);
void setServerDirective(VServerCtx& vsCtx, const std::string& directiveValue);