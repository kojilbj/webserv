NAME = webserv

CORE_SRCS = main.cpp\
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
		ServerCtx.cpp

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
INCLUDEDIRS = -Isrc -Isrc/conf -Isrc/event -Isrc/protocol -Isrc/protocol/phase
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 $(INCLUDEDIRS)

VPATH = src:src/conf:src/event:src/protocol:src/protocol/phase

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

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
