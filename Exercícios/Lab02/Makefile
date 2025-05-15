all: compila

compila:
	gcc *.c -o main

teste:
	./main <teste.txt >saida.txt

valgrind:
	valgrind ./main <teste.txt

clear:
	rm -rf main *.o
	clear