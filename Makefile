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
              $(SRC_DIR)/dongle.c \
              $(SRC_DIR)/scheduler.c \
              $(SRC_DIR)/monito…nsures the directory exists before compiling (Order-only prerequisite)
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
