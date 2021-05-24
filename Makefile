make_sha: main.o sha.o
	gcc main.o sha.o -o main.out

windows: main.o sha.o
	gcc main.o sha.o -o main.exe

main.o: main.c
	gcc -c main.c

sha.o: sha.h sha.c 
	gcc -c sha.c

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	rm -f *.exe.stackdump