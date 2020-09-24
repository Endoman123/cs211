all: first

first: clean first.c
	gcc -g -Wall -Werror -fsanitize=address -std=c11 first.c -o first

test: test1 test2 test3 test4 test5

test1: first
	./first test1.txt | diff result1.txt -

test2: first
	./first test2.txt | diff result2.txt -

test3: first
	./first test3.txt | diff result3.txt -

test4: first
	./first test4.txt | diff result4.txt -

test5: first
	./first test5.txt | diff result5.txt -
clean:
	rm -f first
