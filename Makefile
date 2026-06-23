# --- Program Name ---
NAME        = codexion

# --- Compiler & Flags ---
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread
LDFLAGS     = -pthread
RM          = rm -f

# --- Directories ---
SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = includes

# --- Source Files ---
SRCS        = $(SRC_DIR)/main.c \
							$(SRC_DIR)/simulation.c \
              $(SRC_DIR)/coder.c \
              $(SRC_DIR)/coder_utils.c \
							$(SRC_DIR)/monitor.c \
              $(SRC_DIR)/dongle.c \
							$(SRC_DIR)/dongle_utils.c \
              $(SRC_DIR)/init_dongles.c \
              $(SRC_DIR)/parse_config.c \
              $(SRC_DIR)/heap.c \
              $(SRC_DIR)/heap_utils.c \
              $(SRC_DIR)/init_sim_helpers.c \
              $(SRC_DIR)/init_simulation.c \
              $(SRC_DIR)/simulation_helpers.c \

# --- Object Files (maps src/*.c to obj/*.o) ---
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# --- Include Flags ---
INCLUDES    = -I $(INC_DIR)

# **************************************************************************** #
#                                    Rules                                     #
# **************************************************************************** #

# Default rule
all:        $(NAME)

# Linking rule: Only links if object files are up to date
$(NAME):    $(OBJS)
			$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME)

# Compilation rule: Compiles .c to .o only if the .c file is newer than the .o file
# The '| $(OBJ_DIR)' ensures the directory exists before compiling (Order-only prerequisite)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Directory creation rule
$(OBJ_DIR):
			mkdir -p $(OBJ_DIR)

# Cleanup object files
clean:
			$(RM) -r $(OBJ_DIR)

# Full cleanup: objects + executable
fclean:     clean
			$(RM) $(NAME)

# Rebuild everything from scratch
re:         fclean all

# Prevents Make from confusing these targets with actual files
.PHONY:     all clean fclean re
