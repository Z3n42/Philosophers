# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ingonzal <ingonzal@student.42urduliz.com>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/13 20:12:25 by ingonzal          #+#    #+#              #
#    Updated: 2022/02/05 13:21:13 by ingonzal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = gcc

FLAGS = -Wall -Wextra -Werror -g

AR = ar rcs

RM = rm -f

#S = -fsanitize=address -g3

FILES = ft_philosopher_bonus \
		ft_routine_bonus \
		ft_eat_bonus \

SRCS_DIR = ./
SRCS = $(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))

OBJS_DIR = ./
OBJS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILES)))

.c.o: $(SRCS)
	@$(CC) $(FLAGS) -c -o $@ $<

$(NAME): $(OBJS)
		@$(CC) $(FLAGS) $(S) $^ -o $@ 
		@echo "Preparando Spaguetis"

all: $(NAME)

clean:
		@$(RM) $(OBJS)
		@echo "Recogiendo los platos"

fclean: clean
		@$(RM) $(NAME)
		@echo "Tirando Spaguettis de ayer"

re: clean all

.PHONY: clean fclean %.o
