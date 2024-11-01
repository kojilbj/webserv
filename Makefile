NAME = webserv
SRCS = main.cpp Webserv.cpp ConfFile.cpp confParse.cpp HttpConfCtx.cpp ConfCtx.cpp VServerCtx.cpp Location.cpp setLocationDirective.cpp setVServerDirective.cpp
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

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
