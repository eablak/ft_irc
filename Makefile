# |  |  ___ \    \  |         |
# |  |     ) |  |\/ |   _  |  |  /   _
# ___ __|  __/   |   |  (   |    <    __/
#    _|  _____| _|  _| \__,_| _|\_\ \___|
#                              by jcluzet
################################################################################
#                                     CONFIG                                   #
################################################################################

NAME        := ircserv
CC         := c++
FLAGS      := -Wall -Wextra -Werror -std=c++11 -g #!98
OBJDIR     := objdir

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

SRCDIR     := src
SRCS       := main.cpp \
              checks.cpp\
              error.cpp\
              Client.cpp\
              Numeric.cpp \
              Server.cpp\
              Channel.cpp\
              Utils.cpp \
              HandleMessage.cpp \
              commands/Pass.cpp \
              commands/Nick.cpp \
              commands/User.cpp \
              commands/Join.cpp \
              commands/Topic.cpp\
              commands/Part.cpp\
              commands/Privmsg.cpp\
              commands/Quit.cpp\
              commands/Kick.cpp\
              commands/Names.cpp\
              commands/Cap.cpp\
              commands/Who.cpp\
              commands/Mode.cpp\
              commands/Notice.cpp\
              commands/Ping.cpp\
              fileTransfer.cpp

OBJS       := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

################################################################################
#                                  Makefile  objs                              #
################################################################################

CLR_RMV    := \033[0m
RED        := \033[1;31m
GREEN      := \033[1;32m
YELLOW     := \033[1;33m
BLUE       := \033[1;34m
CYAN       := \033[1;36m
RM         := rm -f

${NAME}: $(OBJDIR) ${OBJS}
	@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
	${CC} ${FLAGS} -o ${NAME} ${OBJS}
	@echo "$(GREEN)$(NAME) created[0m ✔️"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	${CC} ${FLAGS} -c $< -o $@

all: $(OBJDIR) ${NAME}

bonus: all

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@ ${RM} -r $(OBJDIR)
	@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)objs ✔️"

fclean: clean
	@ ${RM} ${NAME}
	@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)binary ✔️"

re: fclean all

.PHONY: all clean fclean re
