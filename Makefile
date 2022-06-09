# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/04 21:26:44 by akhalidy          #+#    #+#              #
#    Updated: 2022/06/09 01:18:49 by mokhames         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY : all clean fclean re

NAME = webserv

SRCS = networking/client.cpp networking/socket.cpp Parsing/Request/Request.cpp FileHandler/fileHandler.cpp main.cpp

FLAGS = -Wall -Werror -Wextra -std=c++98 -g3 -fsanitize=address

$(NAME) : $(SRCS)
		@clang++  $(SRCS) -o $(NAME)
		@echo $(NAME) was created!

all : $(NAME)

clean :
		@rm -rf $(NAME)

fclean : clean

re : fclean all

run : re
		@./$(NAME) Makefile