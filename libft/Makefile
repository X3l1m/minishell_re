# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: seyildir <seyildir@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/11/26 19:08:12 by seyildir      #+#    #+#                  #
#    Updated: 2023/04/30 05:28:12 by seyildir      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

GREEN=\033[32m
RED=\033[31m
NC=\033[0m
################################
NAME	= libft.a
################################
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror
COMPILE = $(CC) $(CFLAGS)
################################
RM		= rm -f
################################
AR		= ar
ARFLAGS	= -rc
ARCHIVE	= $(AR) $(ARFLAGS)
################################
SRC_DIR = ./src
OBJ_DIR = ./obj
################################
SRC 	= $(wildcard $(SRC_DIR)/*.c)
OBJ		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
################################
all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(COMPILE) -c $< -o $@
	@echo "$(GREEN)Compiling $(RED)$(patsubst ft_%,%,$(basename $(notdir $<)))$(NC)"

$(NAME): $(OBJ)
	@echo Creating $(NAME)
	@$(ARCHIVE) $(NAME) $^

clean:
	@echo Cleaning objects files...
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo Deleting $(NAME)
	@$(RM) $(NAME)

re: fclean all

.PHONY: \
	all \
	clean \
	re \
	arc \


# Siyah: \033[0;30m veya \033[30m
# Kırmızı: \033[0;31m veya \033[31m
# Yeşil: \033[0;32m veya \033[32m
# Sarı: \033[0;33m veya \033[33m
# Mavi: \033[0;34m veya \033[34m
# Magenta: \033[0;35m veya \033[35m
# Cyan: \033[0;36m veya \033[36m
# Beyaz: \033[0;37m veya \033[37m
# Bold: \033[1m
# Altı çizili: \033[4m
# Ters: \033[7m
# Sıfırlama: \033[0m