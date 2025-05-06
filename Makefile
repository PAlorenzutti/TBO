all: compila

compila:
	gcc *.c -o main -lm

teste:
	./main <teste.txt

valgrind:
	valgrind ./main <teste.txt

clear:
	rm -rf *.o main
	clear