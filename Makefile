NAME			= webserv

FILES			= main.cpp send_page.cpp socket.cpp create_page.cpp Response.cpp \
					parser_request.cpp backend.cpp parser_conf.cpp \
					process_request.cpp utils.cpp kevent.cpp error.cpp read_request.cpp

INC_PATH		= includes
INC_FILES		= webserv.hpp create_page.hpp send_page.hpp socket.hpp
CLASS_PATH		= class
CLASS_FILES		= Data.hpp Location.hpp Server.hpp Response.hpp
INC				= $(addprefix ${INC_PATH}/, ${INC_FILES}) $(addprefix ${CLASS_PATH}/, ${CLASS_FILES})
SRC_PATH		= srcs
SRC				= $(addprefix ${SRC_PATH}/, ${FILES})

BIN_PATH		= bin
BIN 			= $(SRC:%.cpp=$(BIN_PATH)/%.o)

CC				= clang++
RM				= rm -rf
FLAGS			= -std=c++98 -Wall -Wextra  -Werror

all: 			${NAME}

init:
				@$(shell mkdir -p $(BIN_PATH))

$(BIN): $(BIN_PATH)/%.o: %.cpp ${INC}
				@mkdir -p $(@D)
				@$(CC) $(FLAGS) -I ${INC_PATH} -o $@ -c $<
				@printf "\e[?25l\e[JWEBSERV : \e[92m$(notdir $<)\e[0m\r"

${NAME}:		init ${BIN}
				@${CC} ${FLAGS} ${BIN} -o ${NAME} -I ${INC_PATH}
				@printf '\033[?25l\033[J$(NAME) \033[92m✔ \033[0m\033[?25h\n'

clean:
				@${RM} ${BIN_PATH}
				@printf '\033[?25l\033[J$(NAME) DELETED \033[92m✔ \033[0m\033[?25h\n'

fclean:
				@${RM} ${BIN_PATH}
				@${RM} ${NAME}
				@printf '\033[?25l\033[J$(NAME) DELETED \033[92m✔ \033[0m\033[?25h\n'

re:				fclean all

.PHONY: all clean fclean re bonus init