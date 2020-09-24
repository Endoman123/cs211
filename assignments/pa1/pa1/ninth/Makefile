all: ninth

ninth: ninth.c
	gcc -g -std=c11 -Wall -Werror -fsanitize=address ninth.c -o ninth

clean:
	rm -f ninth

test: ninth test1.txt result1.txt
	./ninth test1.txt | diff - result1.txt
