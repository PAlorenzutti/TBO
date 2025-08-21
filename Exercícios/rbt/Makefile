all: compile

compile:
	gcc *.c -o main 

test_select:
	gcc -Wall -g -o test_select test_select.c rbt.c vector.c queue.c stack.c -lm
	./test_select

clean:
	rm -rf *.o main test_select
	clear