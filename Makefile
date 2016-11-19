LIBS := -lrt -lpthread -g

all: printer client

buffer_queue.o: buffer_queue.c
	gcc $^ -c $(LIBS)

printer: printer.c buffer_queue.o
	gcc $^ -o $@ $(LIBS)

client: client.c buffer_queue.o
	gcc $^ -o $@ $(LIBS)

clean:
	-rm printer client buffer_queue.o
