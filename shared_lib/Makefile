NAME := shim.so

SRC_DIR := ./src/
OBJ_DIR := ./obj/
INC_DIR := ./inc/

SRC := list.c shim.c

CC_FLAGS := -Wall -g
LD_FLAGS := -fPIC -shared

HEADER_FLAGS := -I $(INC_DIR)

CC:=gcc

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LD_FLAGS) $(OBJ) -o $(NAME) -ldl

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c
	@$(CC) $(LD_FLAGS) -c $< -o $@ $(CC_FLAGS) $(HEADER_FLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

re: fclean all

vpath %.c $(SRC_DIR)

.PHONY: all clean fclean re