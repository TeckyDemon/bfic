CC=gcc
CFLAGS=-fsanitize=address -std=c99 -Os -s -pipe -lm -Werror -Wall -Wextra -Wundef -Wshadow -Wconversion -Wunreachable-code -Wfloat-equal -Winit-self -Wformat=2

all: main.o
	$(CC) $? -o bfic $(CFLAGS)
%.o: %.c
	$(CC) -c $? -o $@ $(CFLAGS)