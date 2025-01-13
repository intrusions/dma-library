NAME        := example
CC          := g++
SRCS_DIR    := ./
OBJS_DIR    := .objs
MAIN        := main.cpp
LIB         := ../linux-dma-library/linux-dma-library.a

CFLAGS  :=                                  \
    -DLINUX                                 \
    -I ../linux-dma-library/libs/includes   \
    -I ../linux-dma-library/includes        \

LDFLAGS :=                                      \
    -L ../linux-dma-library/libs/binary         \
    -Wl,-rpath=../linux-dma-library/libs/binary \
    -lleechcore                                 \
    -lvmm                                       \

SRCS :=                                         \

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
