build:
	gcc -Wall -O4 -lutil 2ipty.c -o 2ipty

install: build
	cp 2ipty ~/local/bin
