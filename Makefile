CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99
DEBUG = -g
TARGETS = dealerMain playerTypeA playerTypeB
OFILES = errors.o utility.o path.o game.o player.o coms.o deck.o dealer.o

.DEFAULT_GOAL:= all
.PHONY: all debug clean

all: dealerMain playerTypeA playerTypeB
debug: CFLAGS += $(DEBUG)
debug: clean $(TARGETS)

dealerMain: dealerMain.o $(OFILES)
	$(CC) $(CFLAGS) -o dealerMain dealerMain.o $(OFILES)

playerTypeA: playerTypeA.o playerMain.c $(OFILES)
	$(CC) $(CFLAGS) -o playerTypeA playerTypeA.o playerMain.c $(OFILES)

playerTypeB: playerTypeB.o playerMain.c $(OFILES)
	$(CC) $(CFLAGS) -o playerTypeB playerTypeB.o playerMain.c $(OFILES)

playerMain.o: playerMain.c
	$(CC) $(CFLAGS) -c playerMain.c

playerTypeA.o: playerTypeA.c move.h
	$(CC) $(CFLAGS) -c playerTypeA.c

playerTypeB.o: playerTypeB.c move.h
	$(CC) $(CFLAGS) -c playerTypeB.c

dealerMain.o: dealerMain.c path.h
	$(CC) $(CFLAGS) -c dealerMain.c

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