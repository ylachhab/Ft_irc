NAME = ircserv

FLAGS = -Wall -Wextra -Werror -std=c++98 

CPP = c++

RM = rm -f

OBJF = obj

SRC = $(wildcard *.cpp */*.cpp)
HDR = $(wildcard *.hpp */*.hpp)
OBJ = $(addprefix obj/, $(SRC:.cpp=.o))

all : $(NAME)

$(OBJF):
	mkdir -p $(OBJF)

$(OBJF)/%.o : %.cpp $(HDR) | $(OBJF)
	mkdir -p $(dir $@)
	$(CPP) $(FLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	$(CPP) $(FLAGS) $(OBJ)  -o $(NAME)

clean:
	$(RM) -rf obj
fclean: clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean