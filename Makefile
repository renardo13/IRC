NAME = ircserv

SRCDIR = Srcs
INCDIR = Includes
OBJDIR = objs

SRCS = main.cpp client.cpp server.cpp cpp client.hpp
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)

CC = c++
FLAGS = -Wall -Werror -Wextra -std=c++98 -I$(INCDIR) -MMD -MP

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
