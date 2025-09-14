all: compila
	
compila:
	gcc -o main *.c

valgrind-1:
	valgrind --leak-check=full -s ./main in/in1

valgrind-2:
	valgrind --leak-check=full -s ./main in/in2

clean:
	rm -rf main
	clear
