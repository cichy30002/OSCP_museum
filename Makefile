CC=gcc
museum:  main.o
	$(CC) -pthread -o museum main.o
