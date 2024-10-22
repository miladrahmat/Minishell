# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 11:56:58 by lemercie          #+#    #+#              #
#    Updated: 2024/10/22 15:59:58 by mrahmat-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc

CFLAGS	:= -g -Wextra -Wall -Werror

LDFLAGS	:= -lreadline

NAME	:= minishell

LIBFT	:= ./includes/libft

HEADERS	:= -I $(LIBFT) -I includes/

SRCDIR	:= ./sources/

OBJDIR := ./objects/

SRC_FILES	:= builtin_cmds.c \
			echo.c \
			env_cmd.c \
			builtin_helpers.c \
			environment.c \
			environment1.c \
			export_cmd.c \
			test.c \
			paths.c \
			path_utils.c \
			path_helpers.c \
			split_on_pipes.c \
			cmd_table.c \
			string_utils.c \
			expand_vars.c \
			file_handler.c \
			builtin_error.c \
			tokenizing_utils.c \
			pipe_file_handler.c
			
SRCS	:= $(addprefix $(SRCDIR), $(SRC_FILES))

OBJS	:= $(addprefix $(OBJDIR), $(SRC_FILES:.c=.o))

all: $(OBJDIR) libft $(NAME)

$(OBJDIR):
	@mkdir -p objects

libft:
	@echo "\e[1;93m Compiling Libft ⏳ \e[0;37m"
	@make -s bonus -C $(LIBFT) \
		|| echo "\e[1;31m Failed to compile Libft 😔 \e[0;37m"; exit
	@echo "\e[1;93m Libft compiled! 💪 \e[0;37m"

$(OBJDIR)%.o: $(SRCDIR)%.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) 

$(NAME): $(OBJS) ./includes/minishell.h
	@$(CC) $(LDFLAGS) $(OBJS) $(LIBFT)/libft.a $(HEADERS) -o $(NAME) \
	&& echo "\e[1;92m Minishell ready! \e[0;37m"

clean:
	@rm -rf $(OBJDIR)
	@make -s clean -C $(LIBFT) \
		&& echo "\e[1;96m Removed all object files 🧹 \e[0;37m"

fclean: clean
	@rm -rf $(NAME)
	@make -s fclean -C $(LIBFT) \
		&& echo "\e[1;96m Removed all executables 🧹 \e[0;37m"

re: fclean all

.PHONY: all, clean, fclean, re, libft
