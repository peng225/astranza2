#include "board.h"
#include "history.h"
#include "ai.h"
#include "menu.h"
#include <assert.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>

using std::string;
using std::cout;
using std::endl;
using std::cerr;

const int MAX_HISTORY_NUM = 100;
const int DEFAULT_DEPTH = 14;

int main(int argc, char *argv[])
{
  Board board;
  AI ai, subAI;
  list<History> hist;
  int depth = DEFAULT_DEPTH;

  board.display();

  if(argc >= 2 && argv[1][0] == 'b'){
    BitBoard pos;
    pos = Board::xyToPos(3, 2);
    board.putStone(pos, true);
    pos = Board::xyToPos(2, 2);
    board.putStone(pos, true);
    pos = Board::xyToPos(2, 3);
    board.putStone(pos, true);
    pos = Board::xyToPos(4, 2);
    board.putStone(pos, true);
    pos = Board::xyToPos(2, 1);
    board.putStone(pos, true);
    pos = Board::xyToPos(3, 5);
    board.putStone(pos, true);
    pos = Board::xyToPos(4, 1);
    board.putStone(pos, true);
    pos = Board::xyToPos(3, 1);
    board.putStone(pos, true);
    pos = Board::xyToPos(5, 2);
    board.putStone(pos, true);
    pos = Board::xyToPos(5, 3);
    board.putStone(pos, true);
    pos = Board::xyToPos(3, 0);
    board.putStone(pos, true);
    pos = Board::xyToPos(6, 2);
    board.putStone(pos, true);
    pos = Board::xyToPos(5, 4);
    board.putStone(pos, true);
    pos = Board::xyToPos(5, 5);
    board.putStone(pos, true);
    pos = Board::xyToPos(2, 5);
    board.putStone(pos, true);
    
    board.display();

    int loopNum = 6;
    if(argc == 3){
      loopNum = atoi(argv[2]);
    }

    ai.setTime(INF);        
    for(int i = 0; i < loopNum; i++){
      // Pattern pt;
      // DetailedMoveInfo a = ai.detailedNegascout(board, -INF, INF, 4, pt);
      MoveInfo a = ai.negascout(board, -INF, INF, 4);
      MoveInfo b = ai.minimax(board, 4);
      cout << "target : " << a.score << endl;
      cout << "correct: " << b.score << endl;
      ai.search(board, 6);
    }
    return 0;
  }

  int history_no = 0;
  HIST_ENTRY *history = NULL;
  char* line;
  const char *prompt = "> ";
  while ((line = readline(prompt)) != NULL) {
    string command;
    // std::istringstream* ist;    
    // ist = new std::istringstream(line);    
    // (*ist) >> command;
    list<string> cmdAndArgs;
    boost::split(cmdAndArgs, line, boost::is_space());    
    command = *begin(cmdAndArgs);
    cmdAndArgs.erase(begin(cmdAndArgs));
    if(command == "fight" || command == "f"){
      // fight(board, ai, *ist);
      fight(board, ai, subAI, cmdAndArgs);
    }
    else if(command == "undo" || command == "u"){
      undo(board, hist);
    }
    else if(command == "setDepth" || command == "sd"){
      // (*ist) >> depth;
      if(cmdAndArgs.size() < 1){
	cerr << "The depth is required." << endl;
      }else{
	depth = atoi(begin(cmdAndArgs)->c_str());
	cout << "Depth was changed to " << depth << "." << endl;
      }
    }    
    else if(command == "search" || command == "s"){
      search(board, ai, depth, hist);
    }
    else if(command == "setTime" || command == "st"){
      int time;
      // (*ist) >> time;
      if(cmdAndArgs.size() < 1){
	cerr << "The search time is required." << endl;
      }else{
	time = atoi(begin(cmdAndArgs)->c_str());
	ai.setTime(time);
	cout << "Search time was changed to " << time << "." << endl;
      }
    }
    else if(command == "learn" || command == "l"){
      learn(cmdAndArgs);
      // ln.learn("hoge");
    }
    // else if(command == "generate" || command == "g") gen_kifu(*ist);
    else if(command == "init" || command == "i"){
      board.init();
      hist.clear();
      board.display();
      ai.init();
    }
    else if(command == "put" || command == "p"){
      put(board, hist, cmdAndArgs);
    }
    else if(command == "display" || command == "d") board.display();
    else if(command == "load" || command == "ld"){
      load(ai, subAI, cmdAndArgs);
    }
    // else if(command == "help" || command == "h") help();
    else if(command == "kifu" || command == "k"){
      outputKifu(hist);
    }
    else if(command == "quit" || command == "q") break;    
    // else if(command == "estimate" || command == "es") estimate(*ist);
    else if(command == ""){
    }
    else{
      cerr << "No such command exists." << endl;
    }
    
    // delete ist;
    cmdAndArgs.clear();
    add_history(line);
    if (++history_no > MAX_HISTORY_NUM) {
      history = remove_history(0);
      free(history);
    }
    free(line);
  }
  clear_history();
  return 0;
}
