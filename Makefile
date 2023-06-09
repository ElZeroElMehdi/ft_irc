NAME = ircserv

cpp = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRCS =  ./src/channels.cpp\
		./src/client.cpp\
		./src/main.cpp\
		./src/server.cpp\
		./src/bot.cpp\
		./src/utiles.cpp\
		./src/replies.cpp\
		./src/commands_utills.cpp\
		./src/commands.cpp\
		./src/utills.cpp\
		./src/server_commands.cpp\
		./src/channels_commands.cpp\

headers = ./src/*.hpp


OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(cpp) $(FLAGS) -lcurl $(OBJS) -o $(NAME)

%.o: %.cpp $(headers)
	$(cpp) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:all clean fclean re