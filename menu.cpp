#include "menu.h"

void put(Board &board, list<History> &hist, std::istream &ist){
  // history
  hist.push_back(History(board));

  int x = 0, y = 0;
  bool putSuccess = true;
  if(board.getTurn() == BLACK){
    cout << "Turn:BLACK" << endl;
  }else{
    cout << "Turn:WHITE" << endl;
  }
  ist >> x >> y;

  // 人間視点の座標をコンピュータ視点へ変換
  x--;
  y--;

  // BitBoard revPattern;  
  // if((revPattern = board.putStone(x, y)) == 0){
  if(!board.putStone(x, y)){
    cout << "Illegal move!" << endl;
    putSuccess = false;
    hist.pop_back();
  }
  if(putSuccess){
    board.display();

    // パスの処理
    if(board.isPass()){
      cout << (board.getTurn() == BLACK ? "BLACK" : "WHITE") << " PASS" << endl;
      board.changeTurn();
    }
  }

  
  // board.undo(x, y, revPattern);
  // board.display();  
}

void search(Board &board, AI &ai, int depth, list<History> &hist)
{
  //history
  hist.push_back(History(board));

  if(board.getTurn() == BLACK){
    std::cout << "Turn:BLACK" << std::endl;
  }else{
    std::cout << "Turn:WHITE" << std::endl;
  }
  
  if(!board.isEnd()){
    ai.search(board, depth);
    board.display();

    // パスの処理
    if(board.isPass()){
      cout << (board.getTurn() == BLACK ? "BLACK" : "WHITE") << " PASS" << endl;
      board.changeTurn();
    }
  }
}

void fight(Board &board, AI &ai, std::istream &ist)
{
  list<History> h;
  int depth;

  ist >> depth;
  while(!board.isEnd()){
    search(board, ai, depth, h);
  }
}