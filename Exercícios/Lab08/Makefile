all: compile

compile:
	gcc *.c -o main 

teste:
	./main <teste.txt >saida.txt

valgrind:
	valgrind ./main <teste.txt

clean:
	rm -rf *.o main
	clear