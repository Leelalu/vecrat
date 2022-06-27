CC=gcc
FLAGS=-lrt -lpthread -lX11 -lm -O3 -Wall
FILES=main.c sig_handler.c vecrat.c memsetup.c

vecrat: vecrat.c memsetup.h
	$(CC)  -o vecrat $(FILES) $(FLAGS)
debug:
	$(CC)  -o vecrat $(FILES) $(FLAGS) -g
install:
	sudo $(CC)  -o /usr/local/bin/vecrat $(FILES) $(FLAGS)
clean:
	if [ -f "./vecrat" ];then \
		rm vecrat; \
	fi; \
