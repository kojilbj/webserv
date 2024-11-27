#include "ConfParse.hpp"
#include <gtest/gtest.h>

sig_atomic_t sigInterrupt = 0;

void sigintHandler(int signal)
{
	(void)signal;
	sigInterrupt = 1;
}
using namespace Wbsv;

TEST(ConfFileTest, NoExistFile)
{
	// EXPECT_THROW(
	// 	{ std::vector<ConfCtx*> ctxs = ConfParse::confParse("InvalidFile/noexist.conf"); },
	// 	NoExistFileException);
	try
	{
		std::vector<ConfCtx*> ctxs = ConfParse::confParse("InvalidFile/noexist.conf");
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(e.what(), "No Exist File: no");
	}
}

TEST(ConfFileTest, EmptyFile)
{
	EXPECT_THROW(
		{ std::vector<ConfCtx*> ctxs = ConfParse::confParse("InvalidFile/empty.conf"); },
		EmptyFileException);
}

TEST(ConfFileTest, OnlyHttpBlockFile)
{
	EXPECT_THROW(
		{ std::vector<ConfCtx*> ctxs = ConfParse::confParse("InvalidFIle/onlyHttpBlock.conf"); },
		NotEnoughInfoFileException);
}

TEST(ConfFileTest, HttpDirectiveFile)
{
	EXPECT_THROW(
		{ std::vector<ConfCtx*> ctxs = ConfParse::confParse("InvalidFIle/HttpDirective.conf"); },
		NotEnoughInfoFileException);
}

TEST(ConfFileTest, InvalidContext)
{
	EXPECT_THROW(
		{ std::vector<ConfCtx*> ctxs = ConfParse::confParse("InvalidFIle/mailContext.conf"); },
		InvalidContextException);
}

TEST(ConfFileTest, UnclosedBrace)
{
	EXPECT_THROW(
		{ std::vector<ConfCtx*> ctxs = ConfParse::confParse("InvalidFIle/unclosedBrace.conf"); },
		UnclosedBraceException);
}

TEST(ConfFileTest, DuplicatedServerDirective)
{
	EXPECT_THROW(
		{
			std::vector<ConfCtx*> ctxs =
				ConfParse::confParse("InvalidFIle/duplicatedServerDirective.conf");
		},
		DuplicatedDirectiveException);
}

TEST(ConfFileTest, DuplicatedLocationDirective)
{
	EXPECT_THROW(
		{
			std::vector<ConfCtx*> ctxs =
				ConfParse::confParse("InvalidFIle/duplicatedLocationDirective.conf");
		},
		DuplicatedDirectiveException);
}

TEST(ConfFileTest, DuplicatedHttpContext)
{
	EXPECT_THROW(
		{
			std::vector<ConfCtx*> ctxs =
				ConfParse::confParse("InvalidFIle/duplicatedHttpContext.conf");
		},
		DuplicatedDirectiveException);
}

TEST(ConfFileTest, ValidFile)
{
	EXPECT_NO_THROW({
		std::vector<ConfCtx*> ctxs = ConfParse::confParse("validFile/default.conf");
		ASSERT_FALSE(ctxs.empty());
	});
}
