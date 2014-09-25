#include "board.h"
#include "history.h"
#include "ai.h"
#include "menu.h"
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
  list<History> hist;
  int depth = DEFAULT_DEPTH;

  board.display();

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
    // else if(command == "set_depth" || command == "sd") set_depth(*ist);
    else if(command == "search" || command == "s"){
      search(board, ai, depth, hist);
    }
    // else if(command == "set_time" || command == "st") set_time(*ist);
    // else if(command == "learn" || command == "l") learn(*ist);
    // else if(command == "generate" || command == "g") gen_kifu(*ist);
    else if(command == "init" || command == "i"){
      board.init();
      hist.clear();
      board.display();
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
