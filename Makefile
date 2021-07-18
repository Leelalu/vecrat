CC=gcc
FLAGS=-lrt -lpthread -lX11 -lm -O3

vecrat: vecrat.c memsetup.h
	$(CC) $(FLAGS) -o vecrat main.c vecrat.c memsetup.c
debug:
	$(CC) $(FLAGS) -g -o vecrat main.c vecrat.c memsetup.c
install:
	sudo $(CC) $(FLAGS) -o /usr/local/bin/vecrat main.c vecrat.c memsetup.c
clean:
	if [ -f "./vecrat" ];then \
		rm vecrat; \
	fi; \
