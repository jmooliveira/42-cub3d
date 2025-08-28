#makefile
NAME		=	cub3d

#flags
CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra -Iinclude -Ilib/include -IMLX42/include

#directories
SRC_DIR		=	src
OBJ_DIR 	=	obj
LIB_DIR		=	lib
MLX_DIR		=	MLX42

#fonts
SRC			=	$(SRC_DIR)/main.c \
				$(SRC_DIR)/parser.c \
				$(SRC_DIR)/free_config.c \
				$(SRC_DIR)/validate_map.c \
				$(SRC_DIR)/init_game.c \
				
OBJ			= 	$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#library
LIBFT		=	$(LIB_DIR)/libft.a
MLX 		=	$(MLX_DIR)/build/libmlx42.a -ldl -lglfw -pthread -lm

#rules
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "🔨 Compiling $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX) -o $(NAME)
	@echo "✅ Compilation completed!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@echo "📚 Compiling libft..."
	@make -s -C $(LIB_DIR)

#clear
clean:
	@rm -rf $(OBJ_DIR)
	@make -s -C $(LIB_DIR) clean
	@echo "🧹 object removed."

fclean: clean
	@rm -f $(NAME)
	@make -s -C $(LIB_DIR) fclean
	@echo "🗑️ binary removed."

re: fclean all

run: all
	@./$(NAME) maps/valid/basic.cub

val:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cub3d maps/valid/basic.cub

.PHONY: all clean fclean re
