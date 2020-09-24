all: compile

compile: eighth.c
	gcc -g -std=c11 -Wall -Werror -fsanitize=address eighth.c -o eighth

clean:
	rm -f eighth
