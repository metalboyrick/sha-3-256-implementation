make_sha: main.o sha.o
	gcc -g main.o sha.o -o main.out

windows: main.o sha.o
	gcc -g main.o sha.o -o main.exe

main.o: main.c
	gcc -g -c main.c

sha.o: sha.h sha.c 
	gcc -g -c sha.c

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	rm -f *.exe.stackdump