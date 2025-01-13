NAME        := linux-dma-library.a
CC          := g++
SRCS_DIR    := srcs
OBJS_DIR    := .objs

CFLAGS :=                           \
    -DLINUX                         \
    -DDEBUG                         \
    -I ./includes                   \
    -I ./libs/includes              \

SRCS :=                             \
    Memory.cpp                      \
    Logger.cpp                      \

SRCS_OBJS   := $(patsubst %.cpp,$(OBJS_DIR)/%.o,$(SRCS))

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -vp $(dir $@)
	$(CC) \
		$(CFLAGS) \
		-MMD \
		-MP \
		-o $@ \
		-c $<

all: $(NAME)

-include $(SRCS_OBJS:.o=.d)

$(NAME): $(SRCS_OBJS)
	ar rcs $@ $^

clean:
	rm -rf *.dSYM
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
