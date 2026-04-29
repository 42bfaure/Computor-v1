NAME = computor

SRCS = srcs/Computor.cpp srcs/main.cpp

OBJ_DIR = objs
OBJS = $(patsubst srcs/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: srcs/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re