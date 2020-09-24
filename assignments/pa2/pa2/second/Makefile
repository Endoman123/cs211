all: second

second: clean second.c
	gcc -g -Wall -Werror -fsanitize=address -std=c11 second.c -o second

test: test1 test2 test3 test4 test5

test1: second
	./second test1.txt | diff result1.txt -

test2: second
	./second test2.txt | diff result2.txt -

test3: second
	./second test3.txt | diff result3.txt -

test4: second
	./second test4.txt | diff result4.txt -

test5: second
	./second test5.txt | diff result5.txt -
clean:
	rm -f second
