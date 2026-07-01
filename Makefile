NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread
LDFLAGS     = -pthread
RM          = rm -f

SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = includes

SRCS        = $(SRC_DIR)/main.c \
							$(SRC_DIR)/simulation.c \
              $(SRC_DIR)/coder.c \
              $(SRC_DIR)/coder_utils.c \
              $(SRC_DIR)/coder_helpers.c \
							$(SRC_DIR)/monitor.c \
              $(SRC_DIR)/dongle.c \
							$(SRC_DIR)/dongle_utils.c \
              $(SRC_DIR)/init_dongles.c \
              $(SRC_DIR)/parse_config.c \
              $(SRC_DIR)/parse_config_helpers.c \
              $(SRC_DIR)/heap.c \
              $(SRC_DIR)/heap_utils.c \
              $(SRC_DIR)/init_sim_helpers.c \
              $(SRC_DIR)/init_simulation.c \
              $(SRC_DIR)/simulation_helpers.c \


OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

INCLUDES    = -I $(INC_DIR)
HEADERS = $(INC_DIR)/*.h

all:        $(NAME)

$(NAME):    $(OBJS)
			$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME)

# The '| $(OBJ_DIR)' ensures the directory exists before compiling (Order-only prerequisite)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Directory creation rule
$(OBJ_DIR):
			mkdir -p $(OBJ_DIR)

clean:
			$(RM) -r $(OBJ_DIR)

fclean:     clean
			$(RM) $(NAME)

re:         fclean all

.PHONY:     all clean fclean re
