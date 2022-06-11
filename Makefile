# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/04 21:26:44 by akhalidy          #+#    #+#              #
#    Updated: 2022/06/12 00:33:54 by laafilal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY : all clean fclean re

NAME = webserv

SRCS = networking/client.cpp \
		networking/socket.cpp \
		Parsing/Request/Request.cpp \
		Parsing/Config/Config.cpp \
		Parsing/Config/Server.cpp \
		Parsing/Config/Location.cpp \
		FileHandler/fileHandler.cpp \
		Response/Response.cpp\
		utility/utility.cpp\
		main.cpp

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
		@./$(NAME) ngnix.conf
