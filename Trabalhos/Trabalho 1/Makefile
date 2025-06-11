All:
	gcc -Wall -g -o trab1 *.c

teste_1:
	./trab1 in/1.txt 2 saida.txt

teste_2:
	./trab1 in/2.txt 4 saida.txt

teste_3:
	./trab1 in/3.txt 5 saida.txt

teste_4:
	./trab1 in/4.txt 5 saida.txt

teste_5:
	./trab1 in/5.txt 10 saida.txt

valgrind_1:
	valgrind ./trab1 in/1.txt 2 saida.txt

valgrind_2:
	valgrind ./trab1 in/2.txt 4 saida.txt

valgrind_3:
	valgrind ./trab1 in/3.txt 5 saida.txt

valgrind_4:
	valgrind ./trab1 in/4.txt 5 saida.txt

valgrind_5:
	valgrind ./trab1 in/5.txt 10 saida.txt

zip:
	tar -czvf trab1.tar.gz *.h *.c Makefile

clean:
	rm -rf *.o trab1 saida.txt