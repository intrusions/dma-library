NAME        := example
CC          := g++
SRCS_DIR    := ./
OBJS_DIR    := .objs
MAIN        := main.cpp
LIB         := ../dma-library/dma-library.a

CFLAGS  :=                            \
    -DLINUX                           \
    -I ../dma-library/libs/includes   \
    -I ../dma-library/includes        \

LDFLAGS :=                                  \
    -L../dma-library/libs/binary            \
    -Wl,-rpath=../dma-library/libs/binary   \
    -l:leechcore.so                         \
    -l:vmm.so                               \
    -lfmt                                   \

SRCS :=                                     \

SRCS_OBJS := $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS))

$(OBJS_DIR)/%.o:$(SRCS_DIR)/%.c
	@mkdir -vp $(dir $@)
	$(CC) \
		$(CFLAGS) \
		-MMD \
		-MP \
		-o $@ \
		-c $< \

all: $(NAME)

-include  $(SRCS_OBJS:.o=.d)

$(NAME): $(SRCS_OBJS)
	$(CC) \
		$^ \
		$(MAIN) \
		$(LIB) \
		$(LDFLAGS) \
		$(CFLAGS) \
		-o $(NAME) \

clean:
	rm -rf *.dSYM
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
