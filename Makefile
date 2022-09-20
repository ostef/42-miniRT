WINDOWS_NAME = miniRT.exe
UNIX_NAME = miniRT

SRC_DIR = source
SRC_FILES = main.c objects.c intersection.c parsing.c raycast.c
OBJ_DIR = obj
INCLUDE_DIRS = libft ft_math .
LIB_DIRS = libft ft_math
LIBS = ft ft_math

ifdef OS
NAME = $(WINDOWS_NAME)
SRC_FILES += win32_layer/window.c win32_layer/input.c win32_layer/loop.c
else
NAME = $(UNIX_NAME)
SRC_FILES += mlx_layer/window.c mlx_layer/input.c mlx_layer/loop.c
INCLUDE_DIRS += mlx
LIB_DIRS += mlx
LIBS += mlx
endif

ifdef OS	# This environment variable is only defined on Windows

LIB_FILES = libft/libft.lib ft_math/libft_math.lib User32.lib Gdi32.lib
OBJ_FILES = $(SRC_FILES:.c=.obj)
CC = cl
C_FLAGS = $(addprefix -I,$(INCLUDE_DIRS)) -TC -nologo -O2
DEPENDENCIES = miniRT.h libft/libft.lib ft_math/libft_math.lib Makefile

else

OBJ_FILES = $(SRC_FILES:.c=.o)
CC = gcc
C_FLAGS = -Wall -Wextra -Werror $(addprefix -I, $(INCLUDE_DIRS))
DEPENDENCIES = miniRT.h libft/libft.a ft_math/libft_math.a mlx/libmlx.a Makefile

endif

ifdef OS
all: libft ft_math $(NAME)

else

all: libft ft_math mlx $(NAME)

endif

.PRECIOUS: $(OBJ_DIR)/%.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPENDENCIES)
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

.PRECIOUS: $(OBJ_DIR)/%.obj
$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c $(DEPENDENCIES)
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) -c $< -Fo$(dir $@)

ifdef OS

$(NAME): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	cl -nologo -Fe$@ $(addprefix $(OBJ_DIR)/,$(OBJ_FILES)) $(LIB_FILES)

else

$(NAME): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	$(CC) $(C_FLAGS) $(addprefix $(OBJ_DIR)/, $(OBJ_FILES)) libft/libft.a ft_math/libft_math.a -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)

endif

libft:
	@ $(MAKE) -C libft

ft_math:
	@ $(MAKE) -C ft_math

mlx:
	@ $(MAKE) -C mlx

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

tests/%: tests/%.c $(NAME)
	$(CC) $(C_FLAGS) $< $(NAME) -o $@
	./$@
	@rm -f $@

.PHONY: all libft ft_math clean fclean re
