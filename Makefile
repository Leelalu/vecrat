CC=gcc
FLAGS=-lrt -lpthread -lX11 -O3

vecrat: vecrat.c
	$(CC) $(FLAGS) vecrat.c -o vecrat
debug:
	$(CC) -g $(FLAGS) vecrat.c -o vecrat
install:
	sudo $(CC) $(FLAGS) vecrat.c -o /usr/local/bin/vecrat
clean:
	if [ -f "./vecrat" ];then \
		rm vecrat; \
	fi; \
	$(CC) $(FLAGS) vecrat.c -o vecrat
