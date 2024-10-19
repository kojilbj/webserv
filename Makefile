NAME = webserv
SRCS = main.cpp Webserv.cpp HttpConfFile.cpp Listening.cpp Connection.cpp EpollEvent.cpp Http.cpp HttpRequest.cpp
OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
CXX = c++
INCLUDEDIRS = -Isrc -Isrc/conf -Isrc/event -Isrc/protocol
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 $(INCLUDEDIRS)

VPATH = src:src/conf:src/event:src/protocol

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
debug: all

.PHONY: all clean fclean re debug
