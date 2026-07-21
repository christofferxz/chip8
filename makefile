# CFLAGS=-std=c17 -Wall -Wextra -Werror
CFLAGS=-std=c23 -Wall -Wextra -Werror

all:
	gcc chip8.c -o chip8 `sdl2-config --cflags --libs` $(CFLAGS)

debug:
	gcc chip8.c -o chip8 `sdl2-config --cflags --libs` $(CFLAGS) -DDEBUG

debugt:
	gcc tchip8.c -o chip8 `sdl2-config --cflags --libs` $(CFLAGS) -DDEBUG

emcc:
	emcc chip8.c $(CFLAGS) -sUSE_SDL=2 --preload-file roms -o chip8.html