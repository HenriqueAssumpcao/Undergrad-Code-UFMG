all:
	gcc -Wall -c spend_time.c
	gcc -Wall ep1_main.c spend_time.o -o main -lpthread -lm

clean:
	-rm *.o $(objects) main