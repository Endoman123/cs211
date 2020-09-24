all: first

cache.o: cache.c cache.h
	gcc -std=c11 -Wall -Werror -fsanitize=address -g -c cache.c

first: first.c cache.o
	gcc -std=c11 -Wall -Werror -fsanitize=address -lm -g first.c cache.o -o first

clean:
	rm -f first
	rm -f *.o
