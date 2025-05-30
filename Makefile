# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dell <dell@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/28 15:26:33 by mel-bout          #+#    #+#              #
#    Updated: 2025/05/26 22:23:24 by dell             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Werror -Wextra #-g -fsanitize=thread
 
SRCS = 	philo.c ft_atoi.c parsing.c ft_isdigit.c utils.c init.c start_sim.c routine.c

# transforme tt les .c en .o
OBJS = ${SRCS:.c=.o}

NAME = philo

# on call all avec make
all: ${NAME}

${NAME}: ${OBJS}
	@${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
	@${RM} ${OBJS}

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
