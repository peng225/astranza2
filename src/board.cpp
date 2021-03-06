#include "board.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;



State_t LightBoard::getState(BitBoard pos) const
{
  assert(isValidPos(pos));
  BitBoard isBlack, isWhite;
  isBlack = black & pos;
  isWhite = white & pos;
  assert(isBlack == 0 || isWhite == 0);
  
  if(isBlack > 0){
    return BLACK;
  }else if(isWhite > 0){
    return WHITE;
  }else{
    return SPACE;
  }
}



// pair<int, int> LightBoard::transfer(pair<int, int> pos, Direction d) const
// {
//   pair<int, int> newPos;

//   // newPosが盤外にはみ出す場合が考慮されていない
//   switch(d){
//   case LEFT_UP:
//     newPos.first = pos.first - 1;
//     newPos.second = pos.second - 1;
//     break;
//   case UP:
//     newPos.first = pos.first;
//     newPos.second = pos.second - 1;
//     break;
//   case RIGHT_UP:
//     newPos.first = pos.first + 1;
//     newPos.second = pos.second - 1;
//     break;
//   case RIGHT:
//     newPos.first = pos.first + 1;
//     newPos.second = pos.second;
//     break;
//   case RIGHT_DOWN:
//     newPos.first = pos.first + 1;
//     newPos.second = pos.second + 1;
//     break;
//   case DOWN:
//     newPos.first = pos.first;
//     newPos.second = pos.second + 1;
//     break;
//   case LEFT_DOWN:
//     newPos.first = pos.first - 1;
//     newPos.second = pos.second + 1;
//     break;
//   case LEFT:
//     newPos.first = pos.first - 1;
//     newPos.second = pos.second;
//     break;
//   }

//   return newPos;
// }

void LightBoard::display() const
{
  cout << "   ";
  for(int i = 0; i < BOARD_SIZE; i++){
    cout << i + 1 << " ";
  }
  cout << endl;
  
  for(int i = 0; i < BOARD_SIZE; i++){
    cout << i + 1 << " |";
    for(int j = 0; j < BOARD_SIZE; j++){
      BitBoard pos = xyToPos(j, i);
      if(getState(pos) == BLACK){
	cout << "b";
      }else if(getState(pos) == WHITE){
	cout << "w";
      }else{
	cout << " ";
      }
      cout << "|";
    }
    cout << endl;
  }
  cout << "black: " << __builtin_popcountl(black) << ", "
       << "white: " << __builtin_popcountl(white) << endl;
  cout << endl;
}

bool LightBoard::canPut (BitBoard pos, bool opponent) const
{
  if(!isValidPos(pos)){
    return false;
  }   
  
  // 空白の位置にのみ石を置ける
  if(((black | white) & pos) != 0){
    return false;
  }

  BitBoard ME;
  BitBoard OPPONENT;

  if(!opponent){
    ME = (turn == BLACK ? black : white);
    OPPONENT = (turn == BLACK ? white : black);
  }else{
    OPPONENT = (turn == BLACK ? black : white);
    ME = (turn == BLACK ? white : black);
  }

  
  for(int i = 0; i < NUM_DIRECTION; i++){    
    BitBoard tmpRevPattern = 0;
    BitBoard mask = transfer(pos, DIRS[i]);
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


State_t LightBoard::getWinner() const
{
  int blackCount = __builtin_popcountl(black);
  int whiteCount = __builtin_popcountl(white);
  // for(int i = 0; i < BOARD_SIZE; i++){
  //   for(int j = 0; j < BOARD_SIZE; j++){
  //     BitBoard pos = xyToPos(i, j);
  //     if(getState(pos) == BLACK){
  // 	blackCount++;
  //     }else if(getState(pos) == WHITE){
  // 	whiteCount++;
  //     }
  //   }
  // }
  if(blackCount > whiteCount) return BLACK;
  else if(blackCount == whiteCount) return SPACE;
  else return WHITE;
}


bool LightBoard::operator==(const LightBoard &obj) const
{
  if(black == obj.black && white == obj.white){
    assert(tesuu == obj.tesuu);
    assert(turn == obj.turn);
    return true;
  }else{
    return false;
  }
}

void LightBoard::changeTurn()
{
  turn = (turn == BLACK ? WHITE : BLACK);
}

BitBoard LightBoard::transfer(BitBoard oneBit, Direction d)
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

bool LightBoard::isValidPos(BitBoard pos) // const
{
  return (pos != 0) && ((pos & (pos - 1)) == 0);
}

BitBoard LightBoard::xyToPos(int x, int y)
{
  return (MSB_ONLY_64 >> (x + BOARD_SIZE * y));
}

pair<int, int> LightBoard::posToXY(BitBoard pos)
{
  assert(isValidPos(pos));
  pair<int, int> coord;
  int clz = __builtin_clzl(pos);
  coord.first = clz % BOARD_SIZE;
  coord.second = clz / BOARD_SIZE;
  return coord;
}

// 要ユニットテスト
// BitBoard Board::getDoughnut(BitBoard pos) const
// {
//   assert(isValidPos(pos));
//   // XY座標に変換しなくても、先頭にならぶ0のビット数を数えたりすれば
//   // シフト演算だけでいけるのでは？
//   // さらに、ループ回してshift演算しなくても、
//   // 一気にshiftしてしまうこともできるのでは？
//   // その場合反対側に周りこんだビットの処理が面倒だけど、できそう。
//   pair<int, int> coord = posToXY(pos);
//   coord.first--;
//   coord.second--;  

//   BitBoard doughnut = DOUGHNUT;
//   if(coord.first < 0){
//     doughnut = transfer(doughnut, LEFT);
//   }else{
//     for(int i = 0; i < coord.first; i++){
//       doughnut = transfer(doughnut, RIGHT);
//     }
//   }
  
//   if(coord.second < 0){
//     doughnut = transfer(doughnut, UP);
//   }else{
//     for(int i = 0; i < coord.first; i++){
//       doughnut = transfer(doughnut, DOWN);
//     }
//   }
// }

void LightBoard::displayBitBoard(BitBoard bb)
{
  BitBoard one = 1;
  for(int i = 0; i < BOARD_SIZE; i++){
    for(int j = 0; j < BOARD_SIZE; j++){
      if(((one << ((BOARD_SIZE - i - 1) * BOARD_SIZE + (BOARD_SIZE - j - 1))) & bb) != 0){
	cout << "1";
      }else{
	cout << "0";
      }
    }
    cout << endl;
  }
}

void Board::init()
{
  black = INIT_BLACK;
  white = INIT_WHITE;
  turn = BLACK;
  tesuu = 1;

  candList.clear();
  BitBoard pos = xyToPos(2, 2);
  // BitBoard pos = MSB_ONLY_64 >> (2 + 2 * BOARD_SIZE);
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

BitBoard Board::putStone(BitBoard pos, bool clearClds)
{ 
  if(!isValidPos(pos)){
    return 0;
  }
  
  BitBoard revPattern = 0;
  
  // 空白の位置にのみ石を置ける
  if(((black | white) & pos) != 0){
    return 0;
  }

  const BitBoard ME = (turn == BLACK ? black : white);
  const BitBoard OPPONENT = (turn == BLACK ? white : black);

  
  for(int i = 0; i < NUM_DIRECTION; i++){    
    BitBoard tmpRevPattern = 0;
    BitBoard mask = transfer(pos, DIRS[i]);
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
    black ^= pos | revPattern;
    white ^= revPattern;
  }else{
    white ^= pos | revPattern;
    black ^= revPattern;
  }

  forwardUpdateCandList(pos);
  changeTurn();  
  tesuu++;

  if(clearClds){
    candListDiffs.pop();
    assert(candListDiffs.empty());
  }
  
  return revPattern;
}

void Board::undo(BitBoard pos, BitBoard revPattern)
{
  const BitBoard OPPONENT = (turn == BLACK ? white : black);

  if(OPPONENT == black){
    black ^= pos | revPattern;
    white ^= revPattern;
  }else{
    white ^= pos | revPattern;
    black ^= revPattern;
  }
  
  backUpdateCandList(pos);
  changeTurn();
  tesuu--;
}

bool Board::isEnd() const
{
  for(list<BitBoard>::const_iterator itr = begin(candList);
      itr != end(candList); itr++){
    if(canPut(*itr)){
      return false;
    }
  }
  for(list<BitBoard>::const_iterator itr = begin(candList);
      itr != end(candList); itr++){
    if(canPut(*itr, true)){
      return false;
    }
  }
  return true;
}

bool Board::isPass() const
{ 
  for(list<BitBoard>::const_iterator itr = begin(candList);
      itr != end(candList); itr++){
    if(canPut(*itr)){
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

void Board::forwardUpdateCandList(BitBoard pos)
{
  // cout << "f before" << endl;
  // displayCandListPos();
  assert(isValidPos(pos));
  list<BitBoard>::iterator posItr = find(begin(candList), end(candList), pos);
  assert(posItr != end(candList));
  
  posItr = candList.erase(posItr);

  pair<int, int> clDiff;
  // candListから消された場所が元々candListの何番目にいたかという情報を取得
  clDiff.first = distance(begin(candList), posItr);

  int addCount = 0;
  
  for(int i = 0; i < NUM_DIRECTION; i++){
    BitBoard aroundPos = transfer(pos, DIRS[i]);
    // 盤面からはみ出していたらダメ
    if(!isValidPos(aroundPos)){
      continue;
    }
    // スペースかつリスト未登録であれば新規登録
    if(getState(aroundPos) == SPACE &&
       find(begin(candList), end(candList), aroundPos) == end(candList)){
      posItr = candList.insert(posItr, aroundPos);
      posItr++;
      addCount++;
      // candList.push_back(aroundPos);
    }
  }
  // 新たにいくつの要素をcandListに追加したかという情報
  clDiff.second = addCount;
  candListDiffs.push(clDiff);

  // cout << "f after" << endl;
  // displayCandListPos();
}

void Board::backUpdateCandList(BitBoard pos)
{
  // cout << "b before" << endl;
  // displayCandListPos();
  assert(isValidPos(pos));  

  assert(!candListDiffs.empty());
  pair<int, int> clDiff = candListDiffs.top();
  candListDiffs.pop();
  list<BitBoard>::iterator posItr = begin(candList);

  for(int i = 0; i < clDiff.first; i++){
    posItr++;
  }

  for(int i = 0; i < clDiff.second; i++){
    posItr = candList.erase(posItr);
  }

  candList.insert(posItr, pos);  

  // bool erased = false;
  // for(list<BitBoard>::iterator itr = begin(candList);
  //     itr != end(candList); itr++){    
  //   erased = true;
  //   assert(isValidPos(*itr));
  //   for(int i = 0; i < NUM_DIRECTION; i++){      
  //     BitBoard aroundPos = transfer(*itr, DIRS[i]);
  //     // 盤面からはみ出していたらダメ
  //     if(!isValidPos(aroundPos)){
  // 	assert(aroundPos == 0);
  // 	continue;
  //     }
  //     if(getState(aroundPos) != SPACE){
  // 	erased = false;
  // 	break;
  //     }
  //   }
  //   if(erased){
  //     itr = candList.erase(itr);
  //     itr--;
  //   }
  // }

  // candList.push_back(pos);

  
  // cout << "b after" << endl;
  // displayCandListPos();
}

void Board::displayCandListPos()
{
  BitBoard bb = (BitBoard)0;

  for(list<BitBoard>::iterator itr = begin(candList);
      itr != end(candList); itr++){
    bb |= *itr;
  }
  displayBitBoard(bb);
}
