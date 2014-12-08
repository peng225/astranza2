#include "menu.h"

void printWinner(const Board &board)
{
  State_t winner = board.getWinner();
  switch(winner){
  case BLACK:
    cout << "BLACK won." << endl;
    break;
  case WHITE:
    cout << "WHITE won." << endl;
    break;
  case SPACE:
    cout << "Draw." << endl;
    break;
  default:
    cout << "Bad winner data received." << endl;
  }
}

void put(Board &board, list<History> &hist, std::istream &ist){  
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
  BitBoard pos = Board::xyToPos(x, y);
  assert(Board::isValidPos(pos));

  // history
  hist.push_back(History(board, pos));

  // BitBoard revPattern;  
  // if((revPattern = board.putStone(x, y)) == 0){
  if(!board.putStone(pos, true)){
    cout << "Illegal move!" << endl;
    putSuccess = false;
    hist.pop_back();
  }
  if(putSuccess){
    board.display();

    // 終了処理
    if(board.isEnd()){
      printWinner(board);
      return;
    }

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
  Board oldBoard = board;  

  if(board.getTurn() == BLACK){
    std::cout << "Turn:BLACK" << std::endl;
  }else{
    std::cout << "Turn:WHITE" << std::endl;
  }
  
  if(!board.isEnd()){
    BitBoard pos = 0;
    pos = ai.search(board, depth);
    assert(Board::isValidPos(pos));
    hist.push_back(History(oldBoard, pos));
    board.display();

    // 終了処理
    if(board.isEnd()){
      printWinner(board);
      return;
    }

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

void undo(Board &board, list<History> &hist)
{
  if(hist.size() != 0){
    hist.back().recover(board);
    hist.pop_back();
    board.display();
    if(board.getTurn() == BLACK){
      std::cout << "Turn:BLACK" << std::endl;
    }else{
      std::cout << "Turn:WHITE" << std::endl;
    }
  }else{
    cout << "No history exists." << endl;
  }
}

void outputKifu(list<History> &hist)
{

  string filename = "";
  cout << "filename: " << std::flush;
  cin >> filename;
  
  std::ofstream ofs(filename);
  
  for(list<History>::iterator itr = begin(hist);
      itr != end(hist); itr++){
    ofs << Board::posToXY(itr->getPos()).first + 1 << " "
	<< Board::posToXY(itr->getPos()).second + 1
	<< endl;
  }
}
