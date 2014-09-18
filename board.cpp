#include "board.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

// Board::Board()
// {
//   black = INIT_BLACK;
//   white = INIT_WHITE;
// }

State_t Board::getState(int x, int y)
{
  BitBoard isBlack, isWhite;
  isBlack = black & (MSB_ONLY_64 >> (x + y * BOARD_SIZE));
  isWhite = white & (MSB_ONLY_64 >> (x + y * BOARD_SIZE));
  assert(isBlack == 0 || isWhite == 0);
  
  if(isBlack > 0){
    return BLACK;
  }else if(isWhite > 0){
    return WHITE;
  }else{
    return SPACE;
  }
}

BitBoard Board::transfer(BitBoard oneBit, Direction d)
{
  switch(d){
  case LEFT_UP:
    return (oneBit << (BOARD_SIZE + 1)) & RIGHT_EDGE;
  case UP:
    return (oneBit << BOARD_SIZE);
  case RIGHT_UP:
    return (oneBit << (BOARD_SIZE - 1)) & LEFT_EDGE;
  case RIGHT:
    return (oneBit >> 1) & LEFT_EDGE;
  case RIGHT_DOWN:
    return (oneBit >> (BOARD_SIZE + 1)) & LEFT_EDGE;
  case DOWN:
    return (oneBit >> BOARD_SIZE);
  case LEFT_DOWN:
    return (oneBit >> (BOARD_SIZE - 1)) & RIGHT_EDGE;
  case LEFT:
    return (oneBit << 1) & RIGHT_EDGE;
  default:
    return 0xFFFFFFFFFFFFFFFF;
  }
}

int Board::putStone(int x, int y, State_t turn)
{
  // ユーザの入力と実際のメモリ配置は１ずれている
  // int effX = x - 1;
  // int effY = y - 1;
  
  if(x < 0 || BOARD_SIZE <= x ||
     y < 0 || BOARD_SIZE <= y){
    return OUT_OF_RANGE;
  }

  if(turn != BLACK && turn != WHITE){
    return NONSENSE_VALUE;
  }
  
  const BitBoard POS_BIT = (MSB_ONLY_64 >> (x + BOARD_SIZE * y));
  BitBoard revPattern = 0;
  
  // 空白の位置にのみ石を置ける
  if(((black | white) & POS_BIT) != 0){
    return revPattern;
  }

  const BitBoard ME = (turn == BLACK ? black : white);
  const BitBoard OPPONENT = (turn == BLACK ? white : black);

  
  for(int i = 0; i < NUM_DIRECTION; i++){    
    BitBoard tmpRevPattern = 0;
    BitBoard mask = transfer(POS_BIT, DIRS[i]);
    // 相手の石が存在する間ループ
    while(mask != 0 && (mask & OPPONENT) != 0) {
        tmpRevPattern |= mask;
        mask = transfer(mask, DIRS[i]);
    }
    // 上のループでたどった先に自分の石がなければひっくり返せない
    if((mask & ME) != 0){
      revPattern |= tmpRevPattern;
    }
  }

  // 石の反転を適用
  if(revPattern == 0){
    return 0;
  }else if(ME == black){
    black ^= POS_BIT | revPattern;
    white ^= revPattern;
  }else{
    white ^= POS_BIT | revPattern;
    black ^= revPattern;
  }

  return 0;
}

void Board::display()
{
  for(int i = 0; i < BOARD_SIZE; i++){
    cout << "|";
    for(int j = 0; j < BOARD_SIZE; j++){
      if(getState(j, i) == BLACK){
	cout << "b";
      }else if(getState(j, i) == WHITE){
	cout << "w";
      }else{
	cout << " ";
      }
      cout << "|";
    }
    cout << endl;
  }
  cout << endl;
}
