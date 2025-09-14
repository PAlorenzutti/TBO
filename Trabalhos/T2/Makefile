BIN = trab2

all: compile

compile:
	gcc -o $(BIN) $(shell find src -name "*.c")

test:
	valgrind ./trab2 2 10 0,1,4 4,0,1 entradas/1/file1.txt entradas/1/file2.txt entradas/1/out.txt

test2:
	valgrind ./trab2 2 10 0,1,4 4,0,1 entradas/2/file1.txt entradas/2/file2.txt entradas/2/out.txt

valgrind:
	valgrind ./trab2 2 10 1,0 0,2 examples/3/file1.txt examples/3/file2.txt examples/3/out.txt

zip:
	tar -czvf 2023201631,2023101603,2023101962.tar.gz src/ Makefile

unzip:
	tar -xzvf 2023201631,2023101603,2023101962.tar.gz

clean:
	rm -rf trab2 *.txt
	clear