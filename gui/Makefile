CC = gcc
SRC = gui.c main.c bridge.c route.c alien.c path.c scheduler.c cpu.c ./LPTHREAD/lpthread.c ./LPTHREAD/lpthread_mutex.c 
RUN = alienCommunity
CFLAGS =    -I -Wall `pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags` -lm 

all: build run

build: $(src)
	$(CC) $(SRC) -g -o $(RUN)  $(CFLAGS)

clean:
	rm $(RUN)

run:
	./$(RUN)