all: search

search: binarysearch.c
	gcc -std=c11 -Wall -Werror -fsanitize=address binarysearch.c -o binarysearch

clean:
	rm -f binarysearch
