NAME := leaks_detector

INC_DIR := ./inc/
SRC_DIR := ./src/
OBJ_DIR := ./obj/
LIB_SHARED_DIR := ./shared_lib/

SRC := main.c

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

LIB_SHARED = $(LIB_SHARED_DIR)shim.so

CC_FLAGS:= -Wall -g

HEADER_FLAGS := -I $(INC_DIR)

CC:=gcc

all: $(NAME)

$(NAME): $(LIB_SHARED) $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS) $(HEADER_FLAGS)

$(LIB_SHARED):
	make -C $(LIB_SHARED_DIR)

clean:
	rm -f $(OBJ)
	make clean -C $(LIB_SHARED_DIR)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)
	make fclean -C $(LIB_SHARED_DIR)

re: fclean all

vpath %.c $(SRC_DIR)
