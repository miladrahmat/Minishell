# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrahmat- <mrahmat-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 11:56:58 by lemercie          #+#    #+#              #
#    Updated: 2024/10/23 12:14:19 by mrahmat-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc

CFLAGS	:= -g -Wextra -Wall -Werror

LDFLAGS	:= -lreadline

NAME	:= minishell

LIBFTDIR	:= ./includes/libft/

LIBFT	:= $(LIBFTDIR)libft.a

HEADERS	:= -I $(LIBFTDIR) -I includes/

SRCDIR	:= ./sources/

BUILTIN_DIR	:= $(SRCDIR)builtin_cmd/

PARSING_DIR	:= $(SRCDIR)parsing/

ENV_DIR	:= $(SRCDIR)environment/

FILE_HANDLER_DIR := $(SRCDIR)file_handler/

OBJDIR := ./objects/

MAIN_FILES	:= test.c

BUILTIN_FILES	:= builtin_cmds.c \
				echo.c \
				env_cmd.c \
				builtin_helpers.c \
				export_cmd.c \
				builtin_error.c

PARSING_FILES	:= paths.c \
				path_utils.c \
				path_helpers.c \
				split_on_pipes.c \
				cmd_table.c \
				string_utils.c \
				expand_vars.c \
				tokenizing_utils.c

ENV_FILES	:= environment.c \
			environment1.c

FILE_HANDLER_FILES	:= file_handler.c \
					pipe_file_handler.c

SRCS	:= $(addprefix $(SRCDIR), $(MAIN_FILES)) \
		$(addprefix $(BUILTIN_DIR), $(BUILTIN_FILES)) \
		$(addprefix $(PARSING_DIR), $(PARSING_FILES)) \
		$(addprefix $(ENV_DIR), $(ENV_FILES)) \
		$(addprefix $(FILE_HANDLER_DIR), $(FILE_HANDLER_FILES))

OBJS	:= $(addprefix $(OBJDIR), $(MAIN_FILES:.c=.o)) \
		$(addprefix $(OBJDIR), $(BUILTIN_FILES:.c=.o)) \
		$(addprefix $(OBJDIR), $(PARSING_FILES:.c=.o)) \
		$(addprefix $(OBJDIR), $(ENV_FILES:.c=.o)) \
		$(addprefix $(OBJDIR), $(FILE_HANDLER_FILES:.c=.o))

all: $(OBJDIR) $(LIBFT) $(NAME)

$(OBJDIR):
	@mkdir -p objects

$(LIBFT):
	@echo "\e[1;93m Compiling Libft ⏳ \e[0;37m"
	@make -s bonus -C $(LIBFTDIR) \
		|| echo "\e[1;31m Failed to compile Libft 😔 \e[0;37m"; exit
	@echo "\e[1;93m Libft compiled! 💪 \e[0;37m"

$(OBJDIR)%.o: $(SRCDIR)%.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) 

$(OBJDIR)%.o: $(BUILTIN_DIR)%.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(OBJDIR)%.o: $(PARSING_DIR)%.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)
	
$(OBJDIR)%.o: $(ENV_DIR)%.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(OBJDIR)%.o: $(FILE_HANDLER_DIR)%.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS) ./includes/minishell.h
	@$(CC) $(LDFLAGS) $(OBJS) $(LIBFT) $(HEADERS) -o $(NAME) \
	&& echo "\e[1;92m Minishell compiled successfully! \e[0;37m"

clean:
	@rm -rf $(OBJDIR)
	@make -s clean -C $(LIBFTDIR) \
		&& echo "\e[1;96m Removed all object files 🧹 \e[0;37m"

fclean: clean
	@rm -rf $(NAME)
	@make -s fclean -C $(LIBFTDIR) \
		&& echo "\e[1;96m Removed all executables 🧹 \e[0;37m"

re: fclean all

.PHONY: all, clean, fclean, re, libft