NAME := minishell

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := includes
LIBFT_DIR := libft
LIBFT := $(LIBFT_DIR)/libft.a
HEADERS := $(shell find includes -type f -name '*.h')
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SOURCES:.c=.o))

CC := cc
IFLAGS := -I$(INC_DIR) -I$(LIBFT_DIR)
CFLAGS := -Wall -Wextra -Werror
LFLAGS := -L$(LIBFT_DIR) -lft -lreadline -lhistory

GREEN=\033[32m
RED=\033[31m
RESET=\033[0m

all: $(LIBFT) $(NAME)

valgrind: all
	valgrind --tool=memcheck --leak-check=full ./$(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --quiet

$(NAME): $(OBJ_DIR) $(OBJECTS)
	@$(CC) $(OBJECTS) $(LFLAGS) -o $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiling $(RED)$(basename $(notdir $@))$(RESET)"

clean:
	@$(MAKE) clean -C $(LIBFT_DIR) --quiet
	@for file in $(shell find $(OBJ_DIR) -type f -name '*.o'); do \
		rm -f $$file; \
	done

fclean: clean
	@$(MAKE) fclean -C $(LIBFT_DIR) --quiet
	@-rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re