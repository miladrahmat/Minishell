# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 11:56:58 by lemercie          #+#    #+#              #
#    Updated: 2024/10/10 17:20:46 by mrahmat-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc
CFLAGS	:= -g -Wextra -Wall -Werror
LDFLAGS	:= -lreadline
NAME	:= minishell
LIBFT	:= ./includes/libft

HEADERS	:= -I $(LIBFT) -I includes/
SRCDIR	:= ./sources/
SRCS	:= test.c paths.c path_utils.c path_helpers.c \
			$(SRCDIR)builtin_cmds.c \
			$(SRCDIR)echo.c \
			$(SRCDIR)env_cmd.c \
			$(SRCDIR)builtin_helpers.c \
			$(SRCDIR)environment.c \
			$(SRCDIR)environment1.c \
			$(SRCDIR)export_cmd.c
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
