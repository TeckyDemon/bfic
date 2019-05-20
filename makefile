CC=gcc
CFLAGS=-std=c99 -O3 -s -pipe -lm -Werror -Wall -Wextra -Wundef -Wshadow -Wconversion -Wunreachable-code -Wfloat-equal -Winit-self -Wformat=2

all: main.o
	$(CC) $? -o bfic $(CFLAGS)
	$(RM) *.o
%.o: %.c
	$(CC) -c $? -o $@ $(CFLAGS)
