NAME = webserv
SRCS = main.cpp Webserv.cpp HttpConfFile.cpp Listening.cpp Connection.cpp EpollModule.cpp Http.cpp
OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
CXX = c++
INCLUDEDIRS = -Isrc -Isrc/conf -Isrc/event -Isrc/protocol
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 $(INCLUDEDIRS)

VPATH = src:src/core:src/event:src/event/module:src/protocol/http

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
