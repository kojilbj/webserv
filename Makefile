NAME = webserv
SRCS = main.cpp Webserv.cpp ConfParse.cpp HttpConfCtx.cpp ConfCtx.cpp VServerCtx.cpp storeLocationDirective.cpp storeServerDirective.cpp ConfParseUtil.cpp ErrorPages.cpp LocationCtx.cpp HtmlLocationCtx.cpp CgiLocationCtx.cpp ReturnLocationCtx.cpp ServerCtx.cpp
OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
CXX = c++
#CXXFLAGS = -Wall -Wextra -Werror -std=c++98
CXXFLAGS = -std=c++98

VPATH = src:src/core src::src/conf

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

.PHONY: all clean fclean re
