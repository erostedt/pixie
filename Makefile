CC = clang
CFLAGS = -Wall
EXE = pixie
CFILES = main.c src/canvas.c
INCLUDE = include


build:
	$(CC) $(CFLAGS) -o $(EXE) $(CFILES) -I $(INCLUDE)