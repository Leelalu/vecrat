CC=gcc
SERVFLAGS=-lrt -lX11
CLIENTFLAGS=-lrt -lpthread

vecrat: vecratserver.c vecratclient.c
	$(CC) $(CLIENTFLAGS) vecratclient.c -o vecrat
	$(CC) $(SERVFLAGS) vecratserver.c -o vecratserver
clean:
	rm vecrat vecratserver
	$(CC) $(CLIENTFLAGS) vecratclient.c -o vecrat
	$(CC) $(SERVFLAGS) vecratserver.c -o vecratserver
