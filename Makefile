CC=gcc
SERVFLAGS=-lrt -lpthread -lX11
CLIENTFLAGS=-lrt -lpthread
OPTFLAGS=-O3

vecrat: vecratserver.c vecratclient.c
	$(CC) $(OPTFLAGS) $(CLIENTFLAGS) vecratclient.c -o vecrat
	$(CC) $(OPTFLAGS) $(SERVFLAGS) vecratserver.c -o vecratserver
debug:
	$(CC) -g $(CLIENTFLAGS) vecratclient.c -o vecrat
	$(CC) -g $(SERVFLAGS) vecratserver.c -o vecratserver
install:
	sudo $(CC) $(OPTFLAGS) $(CLIENTFLAGS) vecratclient.c -o /usr/local/bin/vecrat
	sudo $(CC) $(OPTFLAGS) $(SERVFLAGS) vecratserver.c -o /usr/local/bin/vecratserver
clean:
	if [ -f "./vecrat" ];then \
		rm vecrat; \
	fi; \
	if [ -f"./vecratserver" ];then \
		rm vecratserver; \
	fi; \
	$(CC) $(OPTFLAGS) $(CLIENTFLAGS) vecratclient.c -o vecrat
	$(CC) $(OPTFLAGS) $(SERVFLAGS) vecratserver.c -o vecratserver
