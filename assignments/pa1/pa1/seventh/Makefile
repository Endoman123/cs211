all: compile

compile: seventh.c
	gcc -g -std=c11 -Wall -Werror -fsanitize=address seventh.c -o seventh

test: compile
	@echo "Hello and welcome to computer architecture" in pig latin is:
	./seventh Hello and welcome to computer architecture

debug: compile
	gdb --args seventh Hello and welcome to computer architecture

clean:
	rm -f seventh
