CC=gcc
SERVFLAGS=-lrt -lpthread -lX11
CLIENTFLAGS=-lrt -lpthread

vecrat: vecratserver.c vecratclient.c
	$(CC) $(CLIENTFLAGS) vecratclient.c -o vecrat
	$(CC) $(SERVFLAGS) vecratserver.c -o vecratserver
debug:
	$(CC) -g $(CLIENTFLAGS) vecratclient.c -o vecrat
	$(CC) -g $(SERVFLAGS) vecratserver.c -o vecratserver
intstall:
	sudo $(CC) $(CLIENTFLAGS) vecratclient.c -o /usr/local/bin/vecrat
	sudo $(CC) $(SERVFLAGS) vecratserver.c -o /usr/local/bin/vecratserver
clean:
	rm vecrat vecratserver
	$(CC) $(CLIENTFLAGS) vecratclient.c -o vecrat
	$(CC) $(SERVFLAGS) vecratserver.c -o vecratserver
