NAME = CONF_TEST 

CORE_SRCS = gtest_main.cpp\
	   Webserv.cpp\
	   Listening.cpp\
	   Connection.cpp\
	   log.cpp

CONF_SRCS = ConfParse.cpp\
		HttpConfCtx.cpp\
		VServerCtx.cpp\
		storeLocationDirective.cpp\
		storeServerDirective.cpp\
		ConfParseUtil.cpp\
		ErrorPages.cpp\
		LocationCtx.cpp\
		HtmlLocationCtx.cpp\
		CgiLocationCtx.cpp\
		ReturnLocationCtx.cpp\
		ServerCtx.cpp\
		ConfExceptions.cpp

EVENT_SRCS = EpollEvent.cpp

PROTO_SRCS = Protocol.cpp\
	   Http.cpp\
	   Upstream.cpp\
	   FindConfig.cpp\
	   Rewrite.cpp\
	   Access.cpp\
	   Content.cpp

SRCS = $(CORE_SRCS) $(CONF_SRCS) $(EVENT_SRCS) $(PROTO_SRCS)
OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
CXX = c++
INCLUDEDIRS = -I../../src -I../../src/conf -I../../src/event -I../../src/protocol -I../../src/protocol/phase
CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -pthread $(INCLUDEDIRS) 
LDFLAGS=-lgtest -lgtest_main

VPATH = ../../src:../../src/conf:../../src/event:../../src/protocol:../../src/protocol/phase

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

all: $(NAME)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

DEBUG: CXXFLAGS += -DDEBUG
DEBUG: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

debug: CXXFLAGS += -D DEBUG
debug: $(NAME)

.PHONY: all clean fclean re debug
