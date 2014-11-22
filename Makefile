#Makefile


# CFLAGS = -c -g -O3 -Wall -std=c++0x -flto -DNDEBUG
# FINAL_CFLAGS = -g -O3 -Wall -std=c++0x -flto -DNDEBUG

CFLAGS := -c -g -O3 -Wall -std=c++0x -flto -MMD -MP
FINAL_CFLAGS := -g -O3 -Wall -std=c++0x -flto

# CFLAGS = -c -g -O0 -Wall -std=c++0x
# FINAL_CFLAGS = -g -O0 -Wall -std=c++0x

BINARY := astranza2

SRC_DIR := ./src
OBJ_DIR := ./obj
# DEP_DIR := ./dep

SRCS = $(shell ls $(SRC_DIR)/*.cpp)
OBJS = $(subst $(SRC_DIR), $(OBJ_DIR), $(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)

# SRCS := main.cpp board.cpp jouseki.cpp pattern.cpp ai.cpp history.cpp menu.cpp learner.cpp
# OBJS := $(SRCS:%.cpp=%.o)
# DEPS := $(SRCS:%.cpp=%.d)

INCLUDE = -I ./include


CC := g++
# COMPILE = $(CC) $(CFLAGS) $(INCLUDE)

all: $(BINARY)

-include $(DEPS)

$(BINARY): $(OBJS)
	$(CC) $(FINAL_CFLAGS) $(INCLUDE) -o $@ $^ -lreadline


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $< 

# %.o: %.c
# 	$(CC) $(CFLAGS) $<

# $(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
# 	$(COMPILE) -o $@ $<

# $(OBJ_DIR)/board.o: $(SRC_DIR)/board.cpp
# 	$(COMPILE) -o $@ $<

# $(OBJ_DIR)/jouseki.o: $(SRC_DIR)/jouseki.cpp
# 	$(COMPILE) -o $@ $<

# $(OBJ_DIR)/pattern.o: $(SRC_DIR)/pattern.cpp
# 	$(COMPILE) -o $@ $<

# $(OBJ_DIR)/ai.o: $(SRC_DIR)/ai.cpp
# 	$(COMPILE) -o $@ $<

# $(OBJ_DIR)/history.o: $(SRC_DIR)/history.cpp
# 	$(COMPILE) -o $@ $<

# $(OBJ_DIR)/menu.o: $(SRC_DIR)/menu.cpp
# 	$(COMPILE) -o $@ $<

# $(OBJ_DIR)/learner.o: $(SRC_DIR)/learner.cpp
# 	$(COMPILE) -o $@ $<

# board.o: board.h

clean:
	rm -f $(BINARY) $(OBJS) $(DEPS) $(SRC_DIR)/*~

.PHONY: all clean
