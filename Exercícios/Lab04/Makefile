all: compila

compila:
	gcc *.c -o main

nearly_sorted_100k:
	./main 100000 input/nearly_sorted/100000.txt

nearly_sorted_1kk:
	./main 1000000 input/nearly_sorted/1000000.txt

reverse_sorted_100k:
	./main 100000 input/reverse_sorted/100000.txt

reverse_sorted_1kk:
	./main 1000000 input/reverse_sorted/1000000.txt

sorted_100k:
	./main 100000 input/sorted/100000.txt

sorted_1kk:
	./main 1000000 input/sorted/1000000.txt

unif_rand_100k:
	./main 100000 input/unif_rand/100000.txt

unif_rand_1kk:
	./main 1000000 input/unif_rand/1000000.txt

valgrind:
	valgrind ./main 1000000 input/nearly_sorted/1000000.txt

clear:
	rm -rf main *.o
	clear