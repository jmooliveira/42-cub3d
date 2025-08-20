#makefile

NAME	=	cub3d

CC		=	cc
CFLAGS	=	-Wall -Werror -Wextra -g3
LIBFT	=	./lib
LIBMLX	=	./MLX42

LIBS	=	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm $(LIBFT)/libft.a

HEADER	=	-I ./lib/include -I $(LIBMLX)/build

SRCS	=	./src/main.c \

OBJS	=	$(SRCS:.c=.o)


RM		=	rm -rf

all: libft libmlx $(NAME)

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

libft:
	@make -C $(LIBFT)

$(NAME): $(OBJS) libft
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $< ${HEADER}

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT) clean
	@rm -rf $(LIBMLX)/build

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT) fclean

re: fclean all

val:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cub3d

.PHONY: all clean fclean re
