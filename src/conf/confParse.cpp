#include "HttpConfCtx.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> confTokenizer(std::ifstream& confFile)
{
	std::string lineBuff;
	std::string tokenBuff;
	std::vector<std::string> tokens;

	while (std::getline(confFile, lineBuff, '\n'))
	{
		std::istringstream lineStream(lineBuff);
		//Here error handle for case that the character at end of line isn't '{', '}' or ';'
		//Need refactaring this stupid proccess for spit
		while (std::getline(lineStream, tokenBuff, ' '))
		{
			std::istringstream tokenStream(tokenBuff);
			while (std::getline(tokenStream, tokenBuff, '\t'))
			{
				//I don't know but tokenBuff stores void-string sometime relatively proccess for \t
				if (!tokenBuff.empty())
					tokens.push_back(tokenBuff);
			}
		}
	}
	return tokens;
}

std::map<std::string, std::vector<std::string> > mapConfRelative(void)
{
	size_t i;
	std::map<std::string, std::vector<std::string> > confRelatives;
	const char* blocks[] = {"http", "mail", nullptr};
	const char* httpBlocks[] = {"server", nullptr};
	const char* serverBlocks[] = {
		"listen", "location", "error_page", "client_max_body_size", "server_name", nullptr};
	const char* locationBlocks[] = {"path", "root", "index", "limit_except", "autoindex", nullptr};

	i = 0;
	confRelatives["_"];
	while (blocks[i] != nullptr)
	{
		confRelatives["_"].push_back(blocks[i]);
		i++;
	}
	confRelatives["http"];
	i = 0;
	while (httpBlocks[i] != nullptr)
	{
		confRelatives["http"].push_back(httpBlocks[i]);
		i++;
	}
	i = 0;
	confRelatives["server"];
	while (serverBlocks[i] != nullptr)
	{
		confRelatives["server"].push_back(serverBlocks[i]);
		i++;
	}
	i = 0;
	confRelatives["location"];
	while (locationBlocks[i] != nullptr)
	{
		confRelatives["location"].push_back(locationBlocks[i]);
		i++;
	}
	return confRelatives;
}

void printStack(const std::vector<std::string>& stack)
{
	std::vector<std::string>::const_iterator it;

	it = stack.begin();
	while (it != stack.end())
	{
		std::cout << *it << std::endl;
		it++;
	}
}

bool inspectStructure(const std::string& name,
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
	return false;
}

std::string keyValue(std::vector<std::string>::const_iterator it,
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
	return line;
}

int countSpace(const std::string& line)
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

//http以外のブロックを追加するときはここに処理を追加していく
ConfCtx* createCtx(const std::string& ctxName)
{
	ConfCtx* ctx;

	if (ctxName == "http")
		ctx = new HttpConfCtx();
	//if (ctxName == "mail") ctx = new MailConfCtx();
	return ctx;
}

void parseListen(VServerCtx& vsCtx, const std::string& value)
{
	std::vector<std::string> str;
	std::string listenPort;
	std::string listenIP;

	if (countSpace(value) != 0)
		throw std::exception();
	if (value.find(':') != std::string::npos)
	{
		listenIP = value.substr(0, value.find(":"));
		listenPort = value.substr(value.find(":") + 1);
		vsCtx.setListenIP(listenIP);
		vsCtx.setListenPort(std::atoi(listenPort.c_str()));
	}
	else
	{
		listenPort = value.substr(value.find(' ') + 1);
		vsCtx.setListenPort(std::atoi(listenPort.c_str()));
	}
}

void parseServerName(VServerCtx& vsCtx, const std::string& value)
{
	std::stringstream sstr(value);
	std::string buff;

	while (std::getline(sstr, buff, ' '))
		vsCtx.addServerName(buff);
}

void parseClientMaxBodySize(VServerCtx& vsCtx, const std::string& value)
{
	if (countSpace(value) != 0)
		throw std::exception();
	vsCtx.setClientMaxBodySize(std::atoi(value.substr(value.find(' ') + 1).c_str()));
}

void setServerDirective(VServerCtx& vsCtx, const std::string& directiveValue)
{
	std::string key;
	std::string value;

	key = directiveValue.substr(0, directiveValue.find(" "));
	value = directiveValue.substr(directiveValue.find(" ") + 1);
	if (key == "listen")
		parseListen(vsCtx, value);
	if (key == "server_name")
		parseServerName(vsCtx, value);
	if (key == "client_max_body_size")
		parseClientMaxBodySize(vsCtx, value);
}

void setLocationDirective(Location& location, std::string directiveValue)
{
	std::string key;
	std::vector<std::string> values;

	key = directiveValue.substr(directiveValue.find(" "));
	std::cout << key << std::endl;
	if (key == "path")
		;
	if (key == "root")
		;
	if (key == "root")
		;
}

void parser(const std::vector<std::string>& tokens,
			const std::map<std::string, std::vector<std::string> > confRelatives)
{
	std::stack<std::string> blockStack;
	std::vector<std::string>::const_iterator it;
	std::string directiveValue;
	std::vector<ConfCtx*> ctxs;
	HttpConfCtx* httpCtx;
	VServerCtx vsCtx;
	bool inspector;
	bool isPushed;

	it = tokens.begin();
	inspector = true;
	blockStack.push("_");
	//ブロックはblockStackに積んでいって、構造解析を行う。
	//スタックにする理由は構造の解析を行うため、セマンティック解析はクラスに情報を入れる前/後に行うことにする
	while (it != tokens.end())
	{
		isPushed = false;
		if (*it == "{")
		{
			blockStack.push(*(it - 1));
			isPushed = true;
		}
		else if (*it == "}")
			blockStack.pop();
		//;があると値なので構造は検査しなくてい(;がない時に構造検査をする)
		else if ((*it).find(";") == std::string::npos)
			inspector = inspectStructure(*it, blockStack.top(), confRelatives);
		//inspectStructureで例外送出にしてもいいかも
		if (inspector == false)
			throw std::exception();
		if (blockStack.size() == 2 && isPushed)
			ctxs.push_back(createCtx(blockStack.top()));
		if (blockStack.top() == "server" && isPushed)
		{
			httpCtx = dynamic_cast<HttpConfCtx*>(ctxs.back());
			httpCtx->addVServerCtx();
		}
		if (blockStack.top() == "location" & isPushed)
		{
			vsCtx = httpCtx->getVServerCtxs().back();
			vsCtx.addLocation();
		}
		//ディレクティブはstackには積まれない
		if (isPushed == false)
		{
			directiveValue = keyValue(it, tokens);
			if (!directiveValue.empty())
			{
				if (blockStack.top() == "server")
					setServerDirective(vsCtx, directiveValue);
				if (blockStack.top() == "location")
					setLocationDirective(vsCtx.getLocations().back(), directiveValue);
			}
			// Iteratorを値の分進める
			it += countSpace(keyValue(it, tokens));
		}
		it++;
	}
}

void confParse(std::string& confFileName)
{
	std::ifstream confFile;
	std::vector<std::string> tokens;
	std::map<std::string, std::vector<std::string> > confRelatives;

	try
	{
		confFile.open(confFileName, std::ios::in);
		tokens = confTokenizer(confFile);
		confRelatives = mapConfRelative();
		parser(tokens, confRelatives);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cerr << "parser error" << '\n';
	}
}