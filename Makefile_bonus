# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/23 11:21:16 by rgarcia           #+#    #+#              #
#    Updated: 2022/05/24 09:49:22 by rgarcia          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a

SRCS	=	./ft_isalpha.c ./ft_isdigit.c ./ft_isalnum.c ./ft_isascii.c ./ft_isprint.c ./ft_strlen.c \
			./ft_memset.c ./ft_bzero.c ./ft_memcpy.c ./ft_memmove.c \
			./ft_strlcpy.c ./ft_strlcat.c \
			./ft_toupper.c ./ft_tolower.c \
			./ft_strchr.c ./ft_strrchr.c ./ft_strncmp.c ./ft_memchr.c ./ft_memcmp.c ./ft_strnstr.c ./ft_atoi.c \
			./ft_calloc.c ./ft_strdup.c \
			./ft_substr.c ./ft_strjoin.c ./ft_strtrim.c ./ft_split.c ./ft_itoa.c ./ft_strmapi.c ./ft_striteri.c \
			./ft_putchar_fd.c ./ft_putstr_fd.c ./ft_putendl_fd.c ./ft_putnbr_fd.c \
			./ft_printf.c ./ft_putchar_retnum.c ./ft_putstr_retnum.c ./ft_putnbr_retnum.c \
			./get_next_line.c ./get_next_line_utils.c \
			./ft_lstnew.c ./ft_lstadd_front.c ./ft_lstsize.c ./ft_lstlast.c ./ft_lstadd_back.c \
			./ft_lstdelone.c ./ft_lstclear.c ./ft_lstiter.c ./ft_lstmap.c

HEADER_FILES = libft.h ft_printf.h get_next_line.h

OBJS	=	${SRCS:.c=.o}

BONUS_OBJS	=	${BONUS:.c=.o}

CC		=	gcc
RM		=	rm -f

CFLAGS	=	-Wall -Wextra -Werror -c -D BUFFER_SIZE=100

%.o : %.c 	${HEADER_FILES} Makefile
			${CC} ${CFLAGS} $< -o $@

all:		$(NAME)

$(NAME):	${OBJS}
			ar rcs ${NAME} $^

bonus:		${OBJS} ${BONUS_OBJS}
			ar rcs ${NAME} $^

clean:
			${RM} ${OBJS} ${BONUS_OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re bonus
