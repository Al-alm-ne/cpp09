NAME = PmergeMe

CXX := c++
CXXFLAGS += -Wall -Werror -Wextra -g
CXXFLAGS += -std=c++98
CXXFLAGS += -Wshadow
#CXXFLAGS += -Wno-shadow

RM = rm -f
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
GDB = gdb -q --tui

SRCS = main.cpp PmergeMe.cpp
OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)
	@echo "\e[1;33mExecutable file: ./$(NAME)\e[0m"
	@echo "\e[1;45mTo run with valgrind: make val (if no args) or make val ARGS=\"args here\"\e[0m\n"

$(NAME): $(OBJS)
	@echo "\n »» Compiling ««"
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo "\e[1;32mEXERCISE COMPILED.\e[0m"

val:	$(NAME)
	@$(VALGRIND) ./$(NAME) $(ARGS)

gdb:	$(NAME)
	@$(GDB) -ex "break main" -ex "run" --args ./$(NAME) $(ARGS)

clean:
	@$(RM) $(OBJS)
	@echo "\n\e[1;32mObject files removed.\e[0m"

fclean: clean
	@echo "\e[1;33mExecutable file removed.\e[0m"
	@$(RM) $(NAME)
	@echo "\e[1;35mEVERYTHING CLEANED.\e[0m\n"

re: fclean all

.PHONY: all clean fclean re val gdb