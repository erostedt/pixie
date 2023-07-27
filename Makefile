CC = clang
CFLAGS = -Wall
EXE = out/pixie
CFILES = main.c src/core.c src/draw.c src/floodfill.c src/resize.c
INCLUDE = include

build:
	mkdir out
	$(CC) $(CFLAGS) -o $(EXE) $(CFILES) -I $(INCLUDE)

clean:
	rm *.ppm