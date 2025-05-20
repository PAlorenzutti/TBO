all: compila

compila:
	gcc *.c -o main

teste:
	./main 1000 in/nearly_sorted/1Knearly_sorted.txt

valgrind:
	valgrind ./main 1000 in/nearly_sorted/1Knearly_sorted.txt

clear:
	rm -rf main *.o
	clear