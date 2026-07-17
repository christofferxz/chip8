# CFLAGS=-std=c17 -Wall -Wextra -Werror
CFLAGS=-std=c23 -Wall -Wextra -Werror

all:
	gcc chip8.c -o chip8 `sdl2-config --cflags --libs` $(CFLAGS)
# 	gcc test.c -o test `sdl2-config --cflags --libs` $(CFLAGS)

debug:
	gcc chip8.c -o chip8 `sdl2-config --cflags --libs` $(CFLAGS) -DDEBUG