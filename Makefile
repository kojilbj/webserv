NAME = webserv
SRCS = main.cpp\
	   Webserv.cpp\
	   HttpConfCtx.cpp\
	   HtmlLocationCtx.cpp\
	   CgiLocationCtx.cpp\
	   ReturnLocationCtx.cpp\
	   Listening.cpp\
	   Connection.cpp\
	   EpollEvent.cpp\
	   Protocol.cpp\
	   Http.cpp\
	   Upstream.cpp\
	   FindConfig.cpp\
	   Rewrite.cpp\
	   Access.cpp\
	   Content.cpp\

OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
CXX = c++
INCLUDEDIRS = -Isrc -Isrc/conf -Isrc/event -Isrc/protocol -Isrc/protocol/phase
CXXFLAGS = -std=c++98 $(INCLUDEDIRS)
#-Wall -Wextra -Werror 

VPATH = src:src/conf:src/event:src/protocol:src/protocol/phase

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

debug: CXXFLAGS += -D DEBUG
debug: $(NAME)

.PHONY: all clean fclean re debug
