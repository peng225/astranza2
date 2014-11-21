#Makefile

# CFLAGS = -c -g -O3 -Wall -std=c++0x -flto -DNDEBUG
# FINAL_CFLAGS = -g -O3 -Wall -std=c++0x -flto -DNDEBUG

CFLAGS = -c -g -O3 -Wall -std=c++0x -flto
FINAL_CFLAGS = -g -O3 -Wall -std=c++0x -flto

# CFLAGS = -c -g -O0 -Wall -std=c++0x
# FINAL_CFLAGS = -g -O0 -Wall -std=c++0x
BINARY = astranza2
OBJS = board.o jouseki.o pattern.o ai.o history.o menu.o learner.o
COMPILE = g++ $(CFLAGS)

all: $(BINARY)

$(BINARY): main.cpp $(OBJS)
	g++ $(FINAL_CFLAGS) main.cpp $(OBJS) -o $(BINARY) -lreadline

board.o: board.cpp
	$(COMPILE) $<

jouseki.o: jouseki.cpp
	$(COMPILE) $<

pattern.o: pattern.cpp
	$(COMPILE) $<

ai.o: ai.cpp
	$(COMPILE) $<

history.o: history.cpp
	$(COMPILE) $<

menu.o: menu.cpp
	$(COMPILE) $<

learner.o: learner.cpp
	$(COMPILE) $<

board.o: board.h

clean:
	rm -rf $(BINARY) *.o *~