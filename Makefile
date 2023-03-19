# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/09 14:58:51 by abiru             #+#    #+#              #
#    Updated: 2023/03/13 21:59:47 by abiru            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS = philo.c ft_atoi.c get_time.c

CC = gcc -g -O0

CFLAGS = -Wextra -Werror -Wall

OBJS = $(SRCS:.c=.o)

LIBFT = ./libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
		$(CC) $(CFLAGS) -Ilibft $(LIBFT) $(OBJS) -o $(NAME)

$(LIBFT):
	@make -C libft

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make clean -C libft
	rm -rf $(OBJS)

fclean: clean
	@make fclean -C libft
	rm -rf $(NAME)

re: fclean all

.PHONY:	all clean fclean re
