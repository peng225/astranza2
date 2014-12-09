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

void put(Board &board, list<History> &hist, const list<string> &args){
  if(args.size() < 2){
    cerr << "x and y value is required." << endl;
    return;
  }
  
  int x = 0, y = 0;
  bool putSuccess = true;
  if(board.getTurn() == BLACK){
    cout << "Turn:BLACK" << endl;
  }else{
    cout << "Turn:WHITE" << endl;
  }
  // ist >> x >> y;
  list<string>::const_iterator itr = begin(args);
  x = atoi(itr->c_str());
  itr++;
  y = atoi(itr->c_str());

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

void fight(Board &board, AI &ai, AI &subAI, const list<string> &args)
{
  if(args.size() < 2){
    cerr << "The depth and the number of fight is required."
	 << endl;
    return;
  }
  list<History> h;
  int depth;

  // ist >> depth;
  list<string>::const_iterator itr = begin(args);
  depth = atoi(itr->c_str());
  itr++;
  
  int numFight = atoi(itr->c_str());
  
  bool mainTurn, mainFirst;
  int numMainWin = 0, numSubWin = 0;
  for(int i = 0; i < numFight; i++){
    mainTurn = (i % 2 == 0 ? true : false);
    mainFirst = mainTurn;
    while(!board.isEnd()){
      if(mainTurn){
	search(board, ai, depth, h);
      }else{
	search(board, subAI, depth, h);
      }
      mainTurn = !mainTurn;
    }
    if(board.getWinner() == BLACK){
      if(mainFirst){
	numMainWin++;
      }else{
	numSubWin++;
      }
    }else if(board.getWinner() == WHITE){
      if(mainFirst){
	numSubWin++;
      }else{
	numMainWin++;
      }
    }
    board.init();
    ai.init();
    subAI.init();    
  }
  cout << numFight << " times fight." << endl;
  cout << "Main wins: " << numMainWin << endl;
  cout << "Sub wins: " << numSubWin << endl;
  cout << "Draw: " << numFight - numMainWin - numSubWin << endl;
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

void learn(const list<string> &args)
{
  if(args.size() < 1){
    cerr << "The output file name is required." << endl;
    return;
  }
  Learner ln;
  list<string>::const_iterator itr = begin(args);
  string filename = *itr;
  ln.learn(filename);
}

void load(AI &ai, AI &subAI, const list<string> &args)
{
  if(args.size() < 2){
    cerr << "AI type and weight file name is required." << endl;
    return;
  }
  list<string>::const_iterator itr = begin(args);
  int type = atoi(itr->c_str());
  itr++;
  if(type == MAIN_AI){
    ai.loadWeight(*itr);
  }else if(type == SUB_AI){
    subAI.loadWeight(*itr);
  }else{
    cerr << "Invalid type." << endl;
    return;
  }
}
