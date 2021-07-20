CC=gcc
FLAGS=-lrt -lpthread -lX11 -lm -O3 -Wall
FILES=main.c sig_handler.c vecrat.c memsetup.c

vecrat: vecrat.c memsetup.h
	$(CC) $(FLAGS) -o vecrat $(FILES)
debug:
	$(CC) $(FLAGS) -g -o vecrat $(FILES)
install:
	sudo $(CC) $(FLAGS) -o /usr/local/bin/vecrat $(FILES)
clean:
	if [ -f "./vecrat" ];then \
		rm vecrat; \
	fi; \
