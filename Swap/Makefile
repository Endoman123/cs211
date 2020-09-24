all: swap

swap: swap.c
	gcc -std=c11 -Wall -Werror -fsanitize=address swap.c -o swap

clean:
	rm -f swap
