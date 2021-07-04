CC=gcc
museum:  main.o main1.o
	$(CC) -pthread -o museum main.o
	$(CC) -pthread -o museum1 main1.o
