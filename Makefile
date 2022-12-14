# The OS environment variable is only defined on Windows,
# so this is how we know we're on Windows

SRC_DIR = source
SRC_FILES = main.c camera.c edit_mode.c raycast.c\
	parsing/parsing.c parsing/parse_objects.c parsing/parse_elements.c parsing/parse_info.c parsing/parsing_utils.c parsing/ft_atof.c\
	intersection/sphere.c intersection/cylinder.c intersection/plane.c intersection/object.c\
	objects/add.c objects/remove.c objects/transform.c\
	render/clear_frame.c render/render_pixel.c\
	draw/draw_pixel.c draw/draw_rect.c draw/draw_circle.c\
	color_picker/conversion.c color_picker/update.c color_picker/triangle.c color_picker/draw.c\
	ui_utils/is_inside.c
OBJ_DIR = obj
INCLUDE_DIRS = libft ft_math parsing .
LIB_DIRS = libft ft_math
LIBS = ft ft_math

ifdef OS

SRC_FILES += win32_layer/window.c win32_layer/input.c win32_layer/loop.c win32_layer/threads.c win32_layer/pixel.c

else

SRC_FILES += mlx_layer/window.c mlx_layer/input.c mlx_layer/events.c mlx_layer/input_events.c mlx_layer/loop.c mlx_layer/threads.c

endif

BONUS_SRC_FILES := $(SRC_FILES) render/render_frame_mt.c
SRC_FILES += render/render_frame_st.c

ifdef OS

NAME = miniRT.exe
OBJ_FILES = $(SRC_FILES:.c=.obj)
BONUS_OBJ_FILES = $(BONUS_SRC_FILES:.c=.obj)
LIB_FILES = libft/libft.lib ft_math/libft_math.lib User32.lib Gdi32.lib
CC = cl
C_FLAGS = $(addprefix -I,$(INCLUDE_DIRS)) -TC -nologo -O2 #-Z7
LINK_FLAGS = #-Z7
DEPENDENCIES = miniRT.h libft/libft.lib ft_math/libft_math.lib Makefile

else

NAME = miniRT
OBJ_FILES = $(SRC_FILES:.c=.o)
BONUS_OBJ_FILES = $(BONUS_SRC_FILES:.c=.o)
INCLUDE_DIRS += mlx
LIB_DIRS += mlx
LIBS += mlx
CC = gcc
C_FLAGS = $(addprefix -I, $(INCLUDE_DIRS)) # -Wall -Wextra -Werror
DEPENDENCIES = miniRT.h libft/libft.a ft_math/libft_math.a mlx/libmlx.a Makefile

endif

ifdef OS

all: libft ft_math $(NAME)

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c $(DEPENDENCIES)
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) -c $< -Fo$(dir $@)

$(NAME): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	@$(CC) $(LINK_FLAGS) -nologo -Fe$(NAME) $? $(LIB_FILES)

bonus: libft ft_math bonus_exe

bonus_exe: $(addprefix $(OBJ_DIR)/, $(BONUS_OBJ_FILES))
	@$(CC) $(LINK_FLAGS) -nologo -Fe$(NAME) $? $(LIB_FILES)

else

all: libft ft_math mlx $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPENDENCIES)
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

$(NAME): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	$(CC) $(C_FLAGS) $^ libft/libft.a ft_math/libft_math.a -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)

bonus: libft ft_math mlx bonus_exe

bonus_exe: $(addprefix $(OBJ_DIR)/, $(BONUS_OBJ_FILES))
	$(CC) $(C_FLAGS) $? libft/libft.a ft_math/libft_math.a -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)

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

.PHONY: all libft ft_math clean fclean re bonus bonus_exe
