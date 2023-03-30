# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/09 14:58:51 by abiru             #+#    #+#              #
#    Updated: 2023/03/30 13:17:13 by abiru            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS = philo.c ft_atoi.c parse.c init.c philo_utils.c phases.c terminate.c

CC = gcc -g3 -O0

CFLAGS = -Wextra -Werror -Wall

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -lpthread -o $@

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY:	all clean fclean re
