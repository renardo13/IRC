NAME = ircserv

SRCDIR = Srcs
INCDIR = Includes
OBJDIR = objs

SRCS = main.cpp Client.cpp set_server.cpp utils.cpp msgUtils.cpp join.cpp \
handle_commands.cpp Server.cpp Channel.cpp Command.cpp mode.cpp quit.cpp \
privmsg.cpp registration.cpp kick.cpp motd.cpp topic.cpp invite.cpp part.cpp
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)

CC = c++
FLAGS = -Wall -Werror -Wextra -std=c++98 -g3 -I$(INCDIR) -MMD -MP

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)
