all: first

first: first.c
	gcc -std=c11 -Wall -Werror -fsanitize=address -lm -g first.c -o first

clean:
	rm -f first
