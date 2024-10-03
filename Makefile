# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 11:56:58 by lemercie          #+#    #+#              #
#    Updated: 2024/10/03 16:44:32 by lemercie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc
CFLAGS	:= -g -Wextra -Wall -Werror
LDFLAGS	:= -lreadline
NAME	:= minishell
LIBFT	:= ./includes/libft

HEADERS	:= -I $(LIBFT) -I includes/
SRCDIR	:= ./sources/
SRCS	:= $(addprefix $(SRCDIR),test.c parser.c paths.c path_utils.c \
		   path_helpers.c builtin_cmds.c echo.c env_cmd.c)
OBJS	:= ${SRCS:.c=.o}

all: libft $(NAME)

libft: 
	make bonus -C $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) 

$(NAME): $(OBJS) ./includes/minishell.h
	$(CC) $(LDFLAGS) $(OBJS) $(LIBFT)/libft.a $(HEADERS) -o $(NAME)

clean:
	rm -rf $(OBJS)
	make clean -C $(LIBFT)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(LIBFT)

re: clean all

.PHONY: all, clean, fclean, re, libft
