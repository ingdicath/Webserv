# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: aheister <aheister@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/07/10 09:01:00 by aheister      #+#    #+#                  #
#    Updated: 2022/07/25 13:46:34 by aheister      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #


NAME			=	webserv

CFLAGS			=	-Wall -Werror -Wextra -Iincludes -std=c++98
#CFLAGS			=	-Wall -Werror -Iincludes -std=c++98
CC				=	c++

SRC_DIR			=	src/
OBJ_DIR 		=	obj/

SRC				=	main.cpp\
					Client.cpp\
					Server.cpp\
					Webserver.cpp\

SRC_PATH		=	$(addprefix $(SRC_DIR), $(SRC))
OBJ_FILES		=	$(SRC_PATH:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

RM				=	/bin/rm -rf

all:				$(NAME)

$(NAME):			$(OBJ_FILES)
					$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES)
					@echo "\033[38;5;10m'$(NAME)' executable has been created\n\033[0m"

$(OBJ_DIR)%.o:		$(SRC_DIR)%.cpp
					mkdir -p $(OBJ_DIR)
					$(CC) -c $(CFLAGS) -o $@ $<
					@echo "Objects are created"

clean:
					@$(RM) $(OBJ_DIR)
					@echo "Objects were removed - clean."

fclean: 			clean
					@$(RM) $(NAME)
					@echo "\033[38;5;81m'$(NAME)' executable was removed - fclean\033[0m"

re:
					$(MAKE) fclean
					$(MAKE) all

.PHONY:				all clean fclean re