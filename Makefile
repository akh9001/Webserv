# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/04 21:26:44 by akhalidy          #+#    #+#              #
#    Updated: 2022/06/30 19:33:13 by akhalidy         ###   ########.fr        #
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
		cgi-bin/cgi.cpp\
		main.cpp

FLAGS = -Wall -Werror -Wextra -std=c++98

$(NAME) : $(SRCS)
		@clang++ $(FLAGS) $(SRCS) -o $(NAME)
		@echo $(NAME) was created!

all : $(NAME)

clean :
		@rm -rf $(NAME)

fclean : clean

re : fclean all

run : re
		@./$(NAME) ngnixtest.conf
