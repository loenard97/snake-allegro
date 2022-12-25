CC = gcc
CFLAGS = -I/usr/include/x86_64-linux-gnu -lallegro_main -lallegro_font -lallegro_image -lallegro_primitives -lallegro -Wall -Wextra -Werror -g
SRC = src/main.c
BIN = bin/main

.PHONY: all bin dist build clean


all: bin
	$(CC) $(SRC) -o $(BIN) $(CFLAGS)

bin:
	mkdir -p bin

run:
	./$(BIN)

build: all
	debuild --no-tgz-check
	mkdir -p dist
	mkdir -p dist/debian
	mv ../snake_* dist/debian
	mv ../snake-* dist/debian

clean:
	rm -rf bin dist debian/.debhelper debian/snake
