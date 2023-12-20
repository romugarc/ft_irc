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

CC		=	c++
RM		=	rm -f

CFLAGS	=	-Wall -Wextra -Werror -Wconversion -I$(DIR_INC) -std=c++98
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
