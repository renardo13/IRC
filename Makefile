NAME = server
SRCS = server.cpp
OBJS = $(SRCS:.cpp=.o)
OBJDIR = ./objs
OBJ_PATHS = $(addprefix $(OBJDIR)/, $(OBJS))
DEPS = $(OBJ_PATHS:.o=.d)
CC = c++
FLAGS = -Wall -Werror -Wextra -std=c++98 -MMD -MP

all: $(NAME)

$(NAME): $(OBJ_PATHS)
	$(CC) $(FLAGS) $(OBJ_PATHS) -o $(NAME)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

-include $(DEPS)