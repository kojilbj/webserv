#include "ConfParse.hpp"

using namespace Wbsv;

std::vector<std::string> ConfParse::confTokenizer(std::ifstream& confFile)
{
	std::vector<std::string> line;
	std::vector<std::string> tokensTmp;
	std::vector<std::string> tokens;
	std::vector<std::string> tmp;

	line = ConfParseUtil::split(confFile, '\n');
	for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); it++)
	{
		tmp = ConfParseUtil::split(*it, '\t');
		if (!tmp.empty())
			tokensTmp.insert(tokensTmp.end(), tmp.begin(), tmp.end());
	}
	for (std::vector<std::string>::iterator it = tokensTmp.begin(); it != tokensTmp.end(); it++)
	{
		tmp = ConfParseUtil::split(*it, ' ');
		if (!tmp.empty())
			tokens.insert(tokens.end(), tmp.begin(), tmp.end());
	}
	return tokens;
}

std::map<std::string, std::vector<std::string> > ConfParse::mapConfRelative(void)
{
	size_t i;
	std::map<std::string, std::vector<std::string> > confRelatives;
	const char* blocks[] = {"http", NULL};
	const char* httpBlocks[] = {"server", NULL};
	const char* serverBlocks[] = {
		"listen", "location", "error_page", "client_max_body_size", "server_name", NULL};
	const char* locationBlocks[] = {"path",
									"root",
									"index",
									"limit_except",
									"autoindex",
									"return",
									"cgi_param",
									"cgi_index",
									"cgi_store",
									NULL};

	i = 0;
	confRelatives["_"];
	{
		confRelatives["_"].push_back(blocks[i]);
		i++;
	}
	confRelatives["http"];
	i = 0;
	while (httpBlocks[i] != NULL)
	{
		confRelatives["http"].push_back(httpBlocks[i]);
		i++;
	}
	i = 0;
	confRelatives["server"];
	while (serverBlocks[i] != NULL)
	{
		confRelatives["server"].push_back(serverBlocks[i]);
		i++;
	}
	i = 0;
	confRelatives["location"];
	while (locationBlocks[i] != NULL)
	{
		confRelatives["location"].push_back(locationBlocks[i]);
		i++;
	}
	return confRelatives;
}

bool ConfParse::inspectStructure(const std::string& name,
								 const std::string& parentBlock,
								 std::map<std::string, std::vector<std::string> > confRelatives)
{
	decltype(confRelatives)::iterator parentIt;
	std::vector<std::string> childs;
	std::vector<std::string>::iterator childIt;

	parentIt = confRelatives.find(parentBlock);
	childs = (*parentIt).second;
	childIt = childs.begin();
	while (childIt != childs.end())
	{
		if (*childIt == name)
			return true;
		childIt++;
	}
	throw std::logic_error("Unavailable Block: " + name);
	return false;
}

std::string ConfParse::keyValue(std::vector<std::string>::const_iterator it,
								const std::vector<std::string>& tokens)
{
	std::string line;

	if ((*it) == "{" || (*it) == "}")
		return std::string("");
	line = *it;
	it++;
	while (it != tokens.end())
	{
		line += " " + *it;
		if ((*it) == "{" || (*it) == "}")
			return std::string("");
		if (line.find(";") != std::string::npos)
			break;
		it++;
	}
	line = line.substr(0, line.find(';'));
	return line;
}

//http以外のブロックを追加するときはここに処理を追加していく
ConfCtx* ConfParse::createCtx(const std::string& ctxName)
{
	ConfCtx* ctx = NULL;

	if (ctxName == "http")
		ctx = new HttpConfCtx();
	//if (ctxName == "mail") ctx = new MailConfCtx();
	return ctx;
}

void ConfParse::store2Server(HttpConfCtx& httpCtx, const ConfParseUtil::SServer& serverInfo)
{
	httpCtx.addServer(serverInfo);
}

std::vector<ConfCtx*>
ConfParse::parser(const std::vector<std::string>& tokens,
				  const std::map<std::string, std::vector<std::string> > confRelatives)
{
	std::stack<std::string> blockStack;
	std::vector<std::string>::const_iterator it;
	std::string directiveValue;
	std::vector<ConfCtx*> ctxs;
	HttpConfCtx* httpCtx;
	struct ConfParseUtil::SServer* serverInfo;
	struct ConfParseUtil::SLocation* locationInfo;
	bool isPushed;

	it = tokens.begin();
	blockStack.push("_");
	serverInfo = NULL;
	locationInfo = NULL;
	//ブロックはblockStackに積んでいって、構造解析を行う。
	//スタックにする理由は構造の解析を行うため、セマンティック解析はsetter()に情報を入れる前/後に行うことにする
	while (it != tokens.end())
	{
		isPushed = false;
		if (*it == "{")
		{
			blockStack.push(*(it - 1));
			isPushed = true;
		}
		else if (*it == "}")
		{
			if (blockStack.top() == "server")
			{
				//serverInfoにstoreした情報をServerクラスに入れていく
				httpCtx = dynamic_cast<HttpConfCtx*>(ctxs.back());
				store2Server(*httpCtx, *serverInfo);
				delete serverInfo;
			}
			if (blockStack.top() == "location")
			{
				serverInfo->locations.push_back(*locationInfo);
				delete locationInfo;
				// delete は serverInfoのdeleteで行われるようにする
			}
			blockStack.pop();
		}
		//;があると値なので構造は検査しなくてい(;がない時に構造検査をする)
		else if ((*it).find(";") == std::string::npos)
			inspectStructure(*it, blockStack.top(), confRelatives);
		if (blockStack.size() == 2 && isPushed)
			ctxs.push_back(createCtx(blockStack.top()));
		if (blockStack.top() == "server" && isPushed)
		{
			serverInfo = new struct ConfParseUtil::SServer;
		}
		if (blockStack.top() == "location" && isPushed)
		{
			locationInfo = new struct ConfParseUtil::SLocation;
		}
		//ディレクティブはstackには積まれない
		if (isPushed == false)
		{
			directiveValue = keyValue(it, tokens);
			if (!directiveValue.empty())
			{
				//ここで一旦構造体に情報を詰め込んでおく
				if (blockStack.top() == "server")
					storeServerDirective(*serverInfo, directiveValue);
				//多分Locatinoで行くのは無理だからstructuer作る必要がある
				if (blockStack.top() == "location")
					storeLocationDirective(*locationInfo, directiveValue);
				//serverブロックが終わった時にクラスにaddする。その時にVServerの形成を行う
			}
			// Iteratorを値の分進める
			it += ConfParseUtil::countSpace(keyValue(it, tokens));
		}
		it++;
	}
	return ctxs;
}

std::vector<ConfCtx*> ConfParse::confParse(const std::string& confFileName)
{
	std::ifstream confFile;
	std::vector<std::string> tokens;
	std::map<std::string, std::vector<std::string> > confRelatives;
	std::vector<ConfCtx*> ctxs;

	confFile.open(confFileName, std::ios::in);
	if (!confFile.is_open())
		throw std::invalid_argument("No Such File: " + confFileName);
	tokens = confTokenizer(confFile);
	confRelatives = mapConfRelative();
	ctxs = parser(tokens, confRelatives);
	return ctxs;
}

ConfParse::ConfParse(void) { }

ConfParse::~ConfParse(void) { }