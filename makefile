output: functions.o main.o
	gcc functions.o main.o -o output

functions.o: ./src/functions.c
	gcc -Wall -std=c99 ./src/functions.c -c

main.o: ./src/main.c
	gcc -Wall -std=c99 ./src/main.c -c

clean:
	rm -f *.o output