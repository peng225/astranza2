#pragma once

#include <stdint.h>
#include <list>
#include <algorithm>

using std::list;
using std::pair;

typedef enum State{BLACK = -1, WHITE = 1, SPACE = 0} State_t;
typedef enum Direction{LEFT_UP, UP, RIGHT_UP, RIGHT,
		       RIGHT_DOWN, DOWN,
		       LEFT_DOWN, LEFT} Direction_t;
typedef uint64_t BitBoard;


const BitBoard MSB_ONLY_64 = 0x8000000000000000;
const int BOARD_SIZE = 8;
const int NUM_CELL = 64;
const int NUM_DIRECTION = 8;
const BitBoard INIT_BLACK = 0x0000000810000000;
const BitBoard INIT_WHITE = 0x0000001008000000;
const BitBoard LEFT_EDGE = 0x7f7f7f7f7f7f7f7f;
const BitBoard RIGHT_EDGE = 0xfefefefefefefefe;
const BitBoard DOUGHNUT = 0xe0e0e00000000000;

const Direction_t DIRS[NUM_DIRECTION] = {LEFT_UP, UP, RIGHT_UP, RIGHT,
					 RIGHT_DOWN, DOWN,
					 LEFT_DOWN, LEFT};

/*******************************************
                Error Codes
********************************************/
/* enum putStoneErrCode{OUT_OF_RANGE, NONSENSE_VALUE}; */


class Board
{
 public:
 Board() : black(INIT_BLACK), white(INIT_WHITE), turn(BLACK), tesuu(1)
    {
      init();
    };
  void init();
  State_t getState(BitBoard pos) const;
  BitBoard putStone(BitBoard pos);
  void undo(BitBoard pos, BitBoard revPattern);
  /** Check if you can put a stone on (x, y). */
  bool canPut (BitBoard pos) const;
  /** Check if the game is over or not. */
  bool isEnd() const;
  /** If the player whose turn is "turn" wins, return 1.
   * else if draw, return 0.
   * else return -1.
   */
  State_t getWinner() const;
  /** Check if the player whose turn is "turn" must pass or not. */
  bool isPass() const;
  std::list<BitBoard>& getCl(){return candList;}
  void display() const;
  State_t getTurn() const
  {
    return turn;
  };
  int getTesuu() const
  {
    return tesuu;
  };
  void changeTurn();
  BitBoard getBlack() const
  {
    return black;
  }
  BitBoard getWhite() const
  {
    return white;
  }
  bool operator==(const Board &obj) const;

  static BitBoard transfer(BitBoard oneBit, Direction d);
  static BitBoard xyToPos(int x, int y);
  static pair<int, int> posToXY(BitBoard);
  
 private:
  BitBoard black;
  BitBoard white;
  State_t turn;
  int tesuu;
  list<BitBoard> candList;
  
  void forwardUpdateCandList(BitBoard pos);
  void backUpdateCandList(BitBoard pos);
  bool isValidPos(BitBoard pos) const;
  BitBoard getDoughnut(BitBoard pos) const;
};
