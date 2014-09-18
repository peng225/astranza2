#Makefile

CFLAGS = -c -O3 -Wall
FINAL_CFLAGS = -O3 -Wall
BINARY = astranza2
OBJS = board.o
COMPILE = g++ $(CFLAGS)

all: $(BINARY)

$(BINARY): main.cpp $(OBJS)
	g++ $(FINAL_CFLAGS) main.cpp $(OBJS) -o $(BINARY)

board.o: board.cpp
	$(COMPILE) $<

board.o: board.h

clean:
	rm -rf $(BINARY) *.o *~