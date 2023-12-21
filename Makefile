NAME	=	ircserv

SRCS	=	main.cpp			\
			source/Server.cpp	\
			source/User.cpp		\
			source/Channel.cpp	\
			source/Rep.cpp		\
			source/commands/pass.cpp	\
			source/commands/nick.cpp	\
			source/commands/user.cpp	\
			source/commands/join.cpp	\
			source/commands/mode.cpp	\
			source/commands/kick.cpp

OBJS	=	${SRCS:.cpp=.o}

DIR_INC	=	include

HEADER_FILES	= ${DIR_INC}/ft_irc.hpp		\
				  ${DIR_INC}/Channel.hpp 	\
				  ${DIR_INC}/Commands.hpp	\
				  ${DIR_INC}/Rep.hpp		\
				  ${DIR_INC}/Server.hpp		\
				  ${DIR_INC}/User.hpp

CC		=	c++
RM		=	rm -f

CFLAGS	=	-Wall -Wextra -Werror -Wconversion -I$(DIR_INC) -std=c++98 -g3
all:		${NAME}

%.o : %.cpp 	${HEADER_FILES}
		${CC} ${CFLAGS} -c $< -o ${<:.cpp=.o}

$(NAME):	${OBJS}
			@${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re ${NAME}
