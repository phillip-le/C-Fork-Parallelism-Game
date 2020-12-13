CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99
DEBUG = -g
TARGETS = 2310dealer 2310A 2310B
OFILES = errors.o utility.o path.o game.o player.o coms.o deck.o dealer.o

.DEFAULT_GOAL:= all
.PHONY: all debug clean

all: 2310dealer 2310A 2310B
debug: CFLAGS += $(DEBUG)
debug: clean $(TARGETS)

2310dealer: 2310dealer.o $(OFILES)
	$(CC) $(CFLAGS) -o 2310dealer 2310dealer.o $(OFILES)

2310A: 2310A.o 2310player.c $(OFILES)
	$(CC) $(CFLAGS) -o 2310A 2310A.o 2310player.c $(OFILES)

2310B: 2310B.o 2310player.c $(OFILES)
	$(CC) $(CFLAGS) -o 2310B 2310B.o 2310player.c $(OFILES)

2310player.o: 2310player.c
	$(CC) $(CFLAGS) -c 2310player.c

2310A.o: 2310A.c move.h
	$(CC) $(CFLAGS) -c 2310A.c

2310B.o: 2310B.c move.h
	$(CC) $(CFLAGS) -c 2310B.c

2310dealer.o: 2310dealer.c path.h
	$(CC) $(CFLAGS) -c 2310dealer.c

path.o: path.c path.h utility.h
	$(CC) $(CFLAGS) -c path.c

deck.o: deck.c deck.h utility.h errors.h
	$(CC) $(CFLAGS) -c deck.c

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) -c utility.c

player.o: player.c player.h path.h deck.h errors.h utility.h
	$(CC) $(CFLAGS) -c player.c

errors.o: errors.c errors.h
	$(CC) $(CFLAGS) -c errors.c

game.o: game.c game.h player.h path.h errors.h utility.h deck.h
	$(CC) $(CFLAGS) -c game.c

dealer.o: dealer.c dealer.h game.h deck.h errors.h utility.h
	$(CC) $(CFLAGS) -c dealer.c

coms.o: coms.c coms.h game.h player.h errors.h deck.h utility.h dealer.h
	$(CC) $(CFLAGS) -c coms.c

clean:	
	rm -f $(TARGETS) *.o