CC = clang
CFLAGS = -Wall
EXE = out/pixie
CFILES = main.c src/draw.c src/floodfill.c src/resize.c src/rgb.c
INCLUDE = include

build:
	mkdir -p out
	$(CC) $(CFLAGS) -o $(EXE) $(CFILES) -I $(INCLUDE)

clean:
	rm *.ppm