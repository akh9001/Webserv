# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
<<<<<<< HEAD
#    By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/04 21:26:44 by akhalidy          #+#    #+#              #
#    Updated: 2022/06/09 01:18:49 by mokhames         ###   ########.fr        #
=======
#    By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/04 21:26:44 by akhalidy          #+#    #+#              #
#    Updated: 2022/06/11 09:52:10 by akhalidy         ###   ########.fr        #
>>>>>>> akhalidy
#                                                                              #
# **************************************************************************** #

.PHONY : all clean fclean re

NAME = webserv

<<<<<<< HEAD
SRCS = networking/client.cpp networking/socket.cpp Parsing/Request/Request.cpp FileHandler/fileHandler.cpp main.cpp
=======
SRCS = networking/client.cpp \
		networking/socket.cpp \
		Parsing/Request/Request.cpp \
		Parsing/Config/Config.cpp \
		Parsing/Config/Server.cpp \
		Parsing/Config/Location.cpp \
		FileHandler/fileHandler.cpp \
		main.cpp
>>>>>>> akhalidy

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
<<<<<<< HEAD
		@./$(NAME) Makefile
=======
		@./$(NAME) ngnix.conf
>>>>>>> akhalidy
