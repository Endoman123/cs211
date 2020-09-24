all: compile

compile: sixth.c
	gcc -g -std=c11 -Wall -Werror -fsanitize=address sixth.c -o sixth

test: compile
	@echo Hello and welcome to computer architecture
	@./sixth Hello and welcome to computer architecture
	@echo This is just a test to make sure your program is running correctly
	@./sixth This is just a test to make sure your program is running correctly	
	@echo a
	@./sixth a
	@echo bye
	@./sixth bye

debug: compile
	gdb --args sixth Hello and welcome to computer architecture

clean:
	rm -f sixth
