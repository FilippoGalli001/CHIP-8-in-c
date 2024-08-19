all:
	gcc -o chip8 chip8.c `sdl2-config --cflags --libs`
