#include "board.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

void Board::init()
{
  black = INIT_BLACK;
  white = INIT_WHITE;
  turn = BLACK;
  tesuu = 1;

  candList.clear();
  pair<int, int> pos(2, 2);
  candList.push_back(pos);
  
  pos = transfer(pos, RIGHT);
  candList.push_back(pos);
  
  pos = transfer(pos, RIGHT);
  candList.push_back(pos);
  
  pos = transfer(pos, RIGHT);
  candList.push_back(pos);
  
  pos = transfer(pos, DOWN);
  candList.push_back(pos);
  
  pos = transfer(pos, DOWN);
  candList.push_back(pos);
    
  pos = transfer(pos, DOWN);
  candList.push_back(pos);
  
  pos = transfer(pos, LEFT);
  candList.push_back(pos);
    
  pos = transfer(pos, LEFT);
  candList.push_back(pos);
  
  pos = transfer(pos, LEFT);
  candList.push_back(pos);
  
  pos = transfer(pos, UP);
  candList.push_back(pos);
  
  pos = transfer(pos, UP);
  candList.push_back(pos);

  // for(list<pair<int, int> >::iterator i = begin(candList);
  //     i != end(candList); i++){
  //   cout << i->first << ", " << i->second << endl;
  // }
}

State_t Board::getState(int x, int y) const
{
  assert(0 <= x && x < BOARD_SIZE &&
	 0 <= y && y < BOARD_SIZE);
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

BitBoard Board::transfer(BitBoard oneBit, Direction d) const
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

pair<int, int> Board::transfer(pair<int, int> pos, Direction d) const
{
  pair<int, int> newPos;

  // newPosが盤外にはみ出す場合が考慮されていない
  switch(d){
  case LEFT_UP:
    newPos.first = pos.first - 1;
    newPos.second = pos.second - 1;
    break;
  case UP:
    newPos.first = pos.first;
    newPos.second = pos.second - 1;
    break;
  case RIGHT_UP:
    newPos.first = pos.first + 1;
    newPos.second = pos.second - 1;
    break;
  case RIGHT:
    newPos.first = pos.first + 1;
    newPos.second = pos.second;
    break;
  case RIGHT_DOWN:
    newPos.first = pos.first + 1;
    newPos.second = pos.second + 1;
    break;
  case DOWN:
    newPos.first = pos.first;
    newPos.second = pos.second + 1;
    break;
  case LEFT_DOWN:
    newPos.first = pos.first - 1;
    newPos.second = pos.second + 1;
    break;
  case LEFT:
    newPos.first = pos.first - 1;
    newPos.second = pos.second;
    break;
  }

  return newPos;
}

BitBoard Board::putStone(int x, int y)
{ 
  if(x < 0 || BOARD_SIZE <= x ||
     y < 0 || BOARD_SIZE <= y){
    return 0;
  }
  
  const BitBoard POS_BIT = (MSB_ONLY_64 >> (x + BOARD_SIZE * y));
  BitBoard revPattern = 0;
  
  // 空白の位置にのみ石を置ける
  if(((black | white) & POS_BIT) != 0){
    return 0;
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

  forwardUpdateCandList(x, y);
  changeTurn();  
  tesuu++;
  return revPattern;
}

void Board::display() const
{
  cout << "   ";
  for(int i = 0; i < BOARD_SIZE; i++){
    cout << i + 1 << " ";
  }
  cout << endl;
  
  for(int i = 0; i < BOARD_SIZE; i++){
    cout << i + 1 << " |";
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

void Board::undo(int x, int y, BitBoard revPattern)
{
  const BitBoard POS_BIT = (MSB_ONLY_64 >> (x + BOARD_SIZE * y));
  const BitBoard OPPONENT = (turn == BLACK ? white : black);

  if(OPPONENT == black){
    black ^= POS_BIT | revPattern;
    white ^= revPattern;
  }else{
    white ^= POS_BIT | revPattern;
    black ^= revPattern;
  }
  
  backUpdateCandList(x, y);
  changeTurn();
  tesuu--;
}

bool Board::canPut (int x, int y) const
{
  if(x < 0 || BOARD_SIZE <= x ||
     y < 0 || BOARD_SIZE <= y){
    return false;
  }
  
  const BitBoard POS_BIT = (MSB_ONLY_64 >> (x + BOARD_SIZE * y));
  
  // 空白の位置にのみ石を置ける
  if(((black | white) & POS_BIT) != 0){
    return false;
  }

  const BitBoard ME = (turn == BLACK ? black : white);
  const BitBoard OPPONENT = (turn == BLACK ? white : black);

  
  for(int i = 0; i < NUM_DIRECTION; i++){    
    BitBoard tmpRevPattern = 0;
    BitBoard mask = transfer(POS_BIT, DIRS[i]);
    int oppStoneCount = 0;
    // 相手の石が存在する間ループ
    while(mask != 0 && (mask & OPPONENT) != 0) {
        tmpRevPattern |= mask;
        mask = transfer(mask, DIRS[i]);
	oppStoneCount++;
    }
    /*
      上のループでたどった先に自分の石があり、
      かつひっくり返す石が１つでもあればO.K.
    */
    if((mask & ME) != 0 && oppStoneCount != 0){
      return true;
    }
  }
  
  return false;
}

bool Board::isEnd() const
{
  for(list<std::pair<int, int> >::const_iterator itr = begin(candList);
      itr != end(candList); itr++){
    if(canPut(itr->first, itr->second)){
      return false;
    }
  }
  return true;
}

int Board::isWin() const
{
  int me = 0;
  int op = 0;
  State_t oppTurn = (turn == BLACK ? WHITE : BLACK);
  for(int i = 0; i < BOARD_SIZE; i++){
    for(int j = 0; j < BOARD_SIZE; j++){
      if(getState(i, j) == turn){
	me++;
      }else if(getState(i, j) == oppTurn){
	op++;
      }
    }
  }
  if(me > op) return 1;
  else if(me == op) return 0;
  else return -1;
}

bool Board::isPass() const
{ 
  for(list<std::pair<int, int> >::const_iterator itr = begin(candList);
      itr != end(candList); itr++){
    if(canPut(itr->first, itr->second)){
      return false;
    }
  }

  return true;
}

bool Board::operator==(const Board &obj) const
{
  if(black == obj.black && white == obj.white){
    assert(tesuu == obj.tesuu);
    assert(turn == obj.turn);
    return true;
  }else{
    return false;
  }
}

void Board::changeTurn()
{
  turn = (turn == BLACK ? WHITE : BLACK);
}

void Board::forwardUpdateCandList(int x, int y)
{
  assert(0 <= x && x < BOARD_SIZE &&
	 0 <= y && y < BOARD_SIZE);
  
  pair<int, int> pos(x, y);
  assert(find(begin(candList), end(candList), pos) != end(candList));
  candList.erase(find(begin(candList), end(candList), pos));
  
  for(int i = 0; i < NUM_DIRECTION; i++){
    pair<int, int> aroundPos = transfer(pos, DIRS[i]);
    // 盤面からはみ出していたらダメ
    if(aroundPos.first < 0 || BOARD_SIZE <= aroundPos.first ||
	 aroundPos.second < 0 || BOARD_SIZE <= aroundPos.second){
      continue;
    }
    // スペースかつリスト未登録であれば新規登録
    if(getState(aroundPos.first, aroundPos.second) == SPACE &&
       find(begin(candList), end(candList), aroundPos) == end(candList)){
      candList.push_back(aroundPos);
    }
  }
}

void Board::backUpdateCandList(int x, int y)
{
  assert(0 <= x && x < BOARD_SIZE &&
	 0 <= y && y < BOARD_SIZE);
  
  pair<int, int> pos(x, y);
  candList.push_back(pos);

  bool isErase = false;
  for(list<pair<int, int> >::iterator itr = begin(candList);
      itr != end(candList); itr++){
    if(isErase){
      itr--;
    }
    isErase = true;
    for(int i = 0; i < NUM_DIRECTION; i++){
      pair<int, int> aroundPos = transfer(pos, DIRS[i]);
      // 盤面からはみ出していたらダメ
      if(aroundPos.first < 0 || BOARD_SIZE <= aroundPos.first ||
	 aroundPos.second < 0 || BOARD_SIZE <= aroundPos.second){
	continue;
      }
      if(getState(aroundPos.first, aroundPos.second) != SPACE){
	isErase = false;
	break;
      }      
    }
    if(isErase){
      // 本当はremove_ifテンプレート等を使うべき
      itr = candList.erase(itr);
    }
  }
}

