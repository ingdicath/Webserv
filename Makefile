# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: aheister <aheister@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/07/10 09:01:00 by aheister      #+#    #+#                  #
#    Updated: 2022/08/02 13:12:48 by hlin          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

# CFLAGS	=	-Wall -Werror -Wextra -Iincludes -pedantic -std=c++98
CFLAGS		=	-Wall -Werror -Iincludes -std=c++98
CC			=	c++

SRC_DIR		=	src
OBJ_DIR 	=	obj/

SRC			=	main.cpp \
				utils.cpp \
				Configurator.cpp \
				Client.cpp \
				Server.cpp \
				Webserver.cpp \
				Location.cpp \
				Request.cpp\
				Parser.cpp\
				FileUtils.cpp

# SRC_PATH	=	$(addprefix $(SRC_DIR), $(SRC))
# OBJ_FILES	=	$(SRC_PATH:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)
OBJ_FILES	=	$(addprefix $(OBJ_DIR), $(notdir $(SRC:%.cpp=%.o)))
VPATH 		=	$(subst $(space),:,$(shell find $(SRC_DIR) -type d))
OBJ_FILES	=	$(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))

RM			=	/bin/rm -rf

GREEN		=	\033[38;5;10m
RESET		=	\033[0m
CYAN		=	\033[38;5;81m
YELLOW		=	\033[33;3;8m

all:			$(NAME)

$(NAME):		$(OBJ_FILES)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES)
				@echo "'$(GREEN)$(NAME)' executable has been created\n$(RESET)"

# $(SRC_DIR)%.cpp
$(OBJ_DIR)%.o:	$(notdir %.cpp)
				@mkdir -p $(OBJ_DIR)
				@echo "$(YELLOW)Creating objects for: $<$(RESET)"
				@$(CC) -c $(CFLAGS) -o $@ $<

clean:
				@$(RM) $(OBJ_DIR)
				@echo "Objects were removed - clean."

fclean: 		clean
				@$(RM) $(NAME)
				@echo "$(CYAN)'$(NAME)' executable was removed - fclean.$(RESET)"

re:
				$(MAKE) fclean
				$(MAKE) all

.PHONY:			all clean fclean re