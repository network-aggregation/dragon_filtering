vpath %.c src/
vpath %.o obj/
vpath %.h include/

CC=gcc
CFLAGS=-Wall

aggprefs: main.o network.o list.o routing.o prefixes.o utils.o
	$(CC) $(CFLAGS) -o bin/$@ obj/main.o obj/network.o obj/list.o obj/routing.o obj/prefixes.o obj/utils.o

main.o: main.c globals.h network.h routing.h prefixes.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/main.c

network.o: network.c globals.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/network.c

routing.o: routing.c globals.h list.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/routing.c

prefixes.o: prefixes.c globals.h list.h utils.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/prefixes.c

list.o: list.c globals.h
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/list.c

utils.o: utils.c
	$(CC) $(CFLAGS) -o obj/$@ -I include/ -g -c src/utils.c

clean:
	rm -f bin/aggprefs obj/*.o
