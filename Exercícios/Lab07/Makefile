all: compile

compile:
	gcc -o main *.c

test:
	./main in/teste.txt 15

abra:
	./main in/abra.txt 15

moby:
	./main in/moby.txt 15

tale:
	./main in/tale.txt 15

valgrind-test:
	valgrind ./main in/teste.txt 15

valgrind-abra:
	valgrind ./main in/abra.txt 15

valgrind-tale:
	valgrind ./main in/tale.txt 15

clean:
	rm -rf main *.o
	clear