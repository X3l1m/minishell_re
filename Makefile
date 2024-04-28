NAME	= minishell

CC		= cc
CFLAGS	= -Wall -Werror -Wextra
COMPILE	= $(CC) $(CFLAGS)
INC		= -g -lreadline -lhistory -I includes

SRC_DIR = src
OBJ_DIR = obj

LIBFT	= libft/libft.a

SRC	= $(wildcard $(SRC_DIR)/*.c)

OBJ	= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

all: $(LIBFT) $(NAME)
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	$(COMPILE) $^ $(LIBFT) $(INC) -o $(NAME)

$(OBJ): $(SRC)
	mkdir -p $(OBJ_DIR)
	$(COMPILE) -c $< -o $@ $(LIBFT) $(INC)

$(LIBFT):
	make -C libft

clean: 
	make clean -C libft
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(LIBFT)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re