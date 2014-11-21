#include "board.h"
#include "history.h"
#include "ai.h"
#include "menu.h"
#include "learner.h"
#include <assert.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <sstream>

using std::string;
using std::cout;
using std::endl;
using std::cerr;

const int MAX_HISTORY_NUM = 100;
const int DEFAULT_DEPTH = 6;

// cand_listの初期化をしなければ・・・

int main(int argc, char *argv[])
{
  Board board;
  AI ai;
  Learner ln;
  list<History> hist;
  int depth = DEFAULT_DEPTH;

  board.display();

  // いつかここを直さないといけない
  // if(argc >= 2 && argv[1][0] == 'b'){
  //   board.putStone(3, 2);
  //   board.putStone(2, 2);
  //   board.putStone(2, 3);
  //   board.putStone(4, 2);
  //   board.putStone(2, 1);
  //   board.putStone(3, 5);
  //   board.putStone(4, 1);
  //   board.putStone(3, 1);
  //   board.putStone(5, 2);
  //   board.putStone(5, 3);
  //   board.putStone(3, 0);
  //   board.putStone(6, 2);
  //   board.putStone(5, 4);
  //   board.putStone(5, 5);
  //   board.putStone(2, 5);
  //   board.display();

  //   int loopNum = 6;
  //   if(argc == 3){
  //     loopNum = atoi(argv[2]);
  //   }

  //   ai.setTime(INF);
  //   for(int i = 0; i < loopNum; i++){
  //     ai.search(board, 8);
  //   }
  //   return 0;
  // }

  int history_no = 0;
  HIST_ENTRY *history = NULL;
  char* line;
  const char *prompt = "> ";
  while ((line = readline(prompt)) != NULL) {
    string command;
    std::istringstream* ist;    
    ist = new std::istringstream(line);
    (*ist) >> command;
    if(command == "fight" || command == "f"){
      fight(board, ai, *ist);
    }
    else if(command == "undo" || command == "u"){
      undo(board, hist);
    }
    else if(command == "setDepth" || command == "sd"){
      (*ist) >> depth;
      cout << "Depth was changed to " << depth << "." << endl;
    }    
    else if(command == "search" || command == "s"){
      search(board, ai, depth, hist);
    }
    else if(command == "setTime" || command == "st"){
      int time;
      (*ist) >> time;
      ai.setTime(time);
      cout << "Search time was changed to " << time << "." << endl;
    }
    else if(command == "learn" || command == "l"){
      ln.learn("hoge");
    }
    // else if(command == "generate" || command == "g") gen_kifu(*ist);
    else if(command == "init" || command == "i"){
      board.init();
      hist.clear();
      board.display();
      ai.clearHash();
    }
    else if(command == "put" || command == "p"){
      put(board, hist, *ist);
    }
    else if(command == "display" || command == "d") board.display();
    // else if(command == "undo" || command == "u") undo();
    // else if(command == "load" || command == "ld") load(*ist);
    // else if(command == "help" || command == "h") help();
    else if(command == "quit" || command == "q") break;
    // else if(command == "estimate" || command == "es") estimate(*ist);
    else if(command == ""){
    }
    else{
      cerr << "No such command exists." << endl;
    }
    
    delete ist;
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
