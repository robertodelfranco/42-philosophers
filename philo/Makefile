NAME		=	philo

CC			=	cc

FLAGS		=	-Wall -Werror -Wextra -pthread

SRCS		=	./src/init.c \
				./src/main.c \
				./src/clean.c \
				./src/routine.c \
				./src/states.c \
				./src/utils.c

OBJS		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
OBJ_DIR		=	objs
VPATH		=	./src

all: ${NAME}

${NAME}: ${OBJS}
	@$(CC) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(OBJ_DIR)
	${CC} ${FLAGS} -o $@ -c $<

clean:
	@rm -rf ${OBJS}

fclean:	clean
	@rm -rf ${NAME}

re: fclean all

val:
	valgrind --leak-check=full --show-leak-kinds=all ./philo 5 800 300 200 6

hel:
	valgrind --tool=hellgrind --tool=drd ./philo 5 800 200 200

.PHONY: all clean fclean re val hel