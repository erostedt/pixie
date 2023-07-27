CC = clang
CFLAGS = -Wall
EXE = pixie
CFILES = main.c src/core.c src/draw.c src/floodfill.c src/resize.c
INCLUDE = include


build:
	$(CC) $(CFLAGS) -o $(EXE) $(CFILES) -I $(INCLUDE)