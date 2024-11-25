NAME		= minishell

COREKIT_PATH = ./corekit

INCLUDES = -I$(COREKIT_PATH)/includes -I./includes

SRC = src/main.c \
	src/mshell_management.c \
	src/utils.c \
	src/file_utils.c \
	src/errors.c \
	src/env_vars/free_env_vars.c \
	src/env_vars/get_var_by_key.c \
	src/env_vars/new_var.c \
	src/env_vars/delete_env_var.c \
	src/env_vars/update_var.c \
	src/env_vars/env_to_matrix.c \
	src/env_vars/count_env_vars.c \
	src/env_vars/load_env_vars.c \
	src/parsing/get_commands.c \
	src/parsing/new_command.c \
	src/parsing/parse_arguments.c \
	src/parsing/command_ops.c \
	src/parsing/parsing_utils.c \
	src/parsing/get_main_command.c \
	src/parsing/expansion/expand_arguments.c \
	src/parsing/expansion/expand_part.c \
	src/parsing/redirections/scan_for_redirections.c \
	src/parsing/redirections/heredoc.c \
	src/builtins/pwd.c \
	src/builtins/cd.c \
	src/builtins/env.c \
	src/builtins/unset.c \
	src/builtins/export.c \
	src/builtins/echo.c \
	src/builtins/exit.c \
	src/execution/execution_utils.c \
	src/execution/pipeline_utils.c \
	src/execution/pipeline.c \
	src/execution/execute_command.c \

FLAGS		= -g
FLAGS		+= -Wall -Werror -Wextra

OBJS		= $(SRC:%.c=%.o)

# -lreadline is used to include all function realative to readline()
$(NAME): download $(OBJS)
	@${MAKE} -sC corekit
	@echo "$(GREEN)[COREKIT]:\t\t COREKIT CREATED$(RESET)"
	@cc $(FLAGS) $(OBJS) -L$(COREKIT_PATH) -lcorekit -lreadline -o $(NAME)
	@echo "$(GREEN)[MINISHELL]:\t\t PROJECT COMPILED$(RESET)"

all:$(NAME)

%.o: %.c
	@cc -c $< -o $@ $(FLAGS) $(INCLUDES) -Imlx

clean:
	@rm -fr $(OBJS)
	@${MAKE} -sC corekit clean
	@echo "$(RED)[COREKIT]:\t\t COREKIT CLEAN$(RESET)"
	@echo "$(RED)[MINISHELL]:\t\t CLEAN$(RESET)"

fclean: clean
	@rm -f *.a
	@rm -f **/*.o
	@rm -f *.gch
	@rm -f $(NAME)
	@${MAKE} -sC corekit fclean -s
	@echo "$(RED)[COREKIT]:\t\t COREKIT FCLEAN$(RESET)"
	@echo "$(RED)[MINISHELL]:\t\t FCLEAN$(RESET)"

re: fclean all

remove_mlx:
	@rm -fr mlx

test: all
	 ./$(NAME)
val: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind_suppression.supp -s --track-fds=yes ./$(NAME)

#TO DO mettere corekit statica
download:
	@if [ ! -d "./corekit/" ]; then \
			git clone https://github.com/Sheratan095/Corekit.git corekit > /dev/null 2>&1; \
			echo "$(GREEN)[COREKIT]:\t\t COREKIT DOWNLADED$(RESET)"; \
	else \
			echo "$(BLUE)[COREKIT]:\t\t COREKIT ALREADY EXISTS$(RESET)"; \
	fi

.PHONY: download

#COLORS

GREEN=\033[0;32m
RED=\033[0;31m
BLUE=\033[0;34m
RESET=\033[0m