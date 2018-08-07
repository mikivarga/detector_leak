NAME = leaks_detector

INC_DIR = ./inc/
SRC_DIR = ./src/
OBJ_DIR = ./obj/

CC_FLAGS:= -Wall
HEADER_FLAGS:= -I $(INC_DIR)
CC:=gcc

SRC = main.c

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	@echo $(NAME) ": Source compilling..."
	@$(CC) $(OBJ) -o $(NAME)

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS) $(HEADER_FLAGS)

