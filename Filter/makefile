vpath %.c src/
vpath %.o obj/
vpath %.h include/

CC=gcc
CFLAGS=-Wall

filter: main.o network.o list.o routing.o prefixes.o filter.o
	$(CC) $(CFLAGS) -o bin/$@ obj/main.o obj/network.o obj/list.o obj/routing.o obj/prefixes.o obj/filter.o
	
main.o: main.c globals.h network.h routing.h prefixes.h filter.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/main.c
	
network.o: network.c globals.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/network.c

routing.o: routing.c globals.h list.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/routing.c

prefixes.o: prefixes.c globals.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/prefixes.c

filter.o: filter.c globals.h list.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/filter.c

list.o: list.c globals.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/list.c

clean:
	rm -f bin/filter obj/*.o
