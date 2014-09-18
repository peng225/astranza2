#pragma once

#include <stdint.h>

typedef uint64_t BitBoard;
typedef enum State{BLACK, WHITE, SPACE} State_t;
typedef enum Direction{LEFT_UP, UP, RIGHT_UP, RIGHT,
		       RIGHT_DOWN, DOWN,
		       LEFT_DOWN, LEFT} Direction_t;

const int BOARD_SIZE = 8;
const int NUM_CELL = 64;
const int NUM_DIRECTION = 8;
const BitBoard INIT_BLACK = 0x0000000810000000;
const BitBoard INIT_WHITE = 0x0000001008000000;
const BitBoard MSB_ONLY_64 = 0x8000000000000000;
const BitBoard LEFT_EDGE = 0x7f7f7f7f7f7f7f7f;
const BitBoard RIGHT_EDGE = 0xfefefefefefefefe;
const Direction_t DIRS[NUM_DIRECTION] = {LEFT_UP, UP, RIGHT_UP, RIGHT,
					 RIGHT_DOWN, DOWN,
					 LEFT_DOWN, LEFT};

/*******************************************
                Error Codes
********************************************/
enum putStoneErrCode{OUT_OF_RANGE, NONSENSE_VALUE};


class Board
{
 public:
 Board() : black(INIT_BLACK), white(INIT_WHITE){};
  State_t getState(int x, int y);
  int putStone(int x, int y, State_t turn);
  void display();
 private:
  BitBoard black;
  BitBoard white;
  BitBoard getReversePattern(BitBoard pos);
  BitBoard transfer(BitBoard oneBit, Direction d);
};
