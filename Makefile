# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 11:56:58 by lemercie          #+#    #+#              #
#    Updated: 2024/10/22 11:08:57 by mrahmat-         ###   ########.fr        #
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
			parser.c \
			cmd_table.c \
			string_utils.c \
			expand_vars.c \
			file_handler.c \
			builtin_error.c
			
SRCS	:= $(addprefix $(SRCDIR), $(SRC_FILES))

OBJS	:= $(addprefix $(OBJDIR), $(SRC_FILES:.c=.o))

all: $(OBJDIR) libft $(NAME)

$(OBJDIR):
	mkdir -p objects

libft: 
	make bonus -C $(LIBFT)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) 

$(NAME): $(OBJS) ./includes/minishell.h
	$(CC) $(LDFLAGS) $(OBJS) $(LIBFT)/libft.a $(HEADERS) -o $(NAME)

clean:
	rm -rf $(OBJDIR)
	make clean -C $(LIBFT)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(LIBFT)

re: clean all

.PHONY: all, clean, fclean, re, libft
