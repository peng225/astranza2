#pragma once
#include <vector>
#include <list>
#include <string>
#include "board.h"
#include "pattern.h"
#include "ai.h"

/* const double LEARNING_RATE = 0.207; */
/* const double GAMMA = 0.0717; */
/* const double ACCURACY = 0.327; */

const double LEARNING_RATE = 1.388;
const double GAMMA = 1.437;
const double ACCURACY = 0.899;

const int LOAD_KIFU_NUM = 260;
/* const int LOAD_KIFU_NUM = 1; */

const int REPEAT_NUM = 10;
/* const int REPEAT_NUM = 2; */

const int THRESH_NUM_CUTOFF_MOVE = 4;
const int MAX_FILENAME = 32;

const int SHALLOW = 3;
const int DEEP = 5;

using std::vector;
using std::list;
using std::pair;

//学習する重みは黒のためのもの。白の場合は符号を逆転して使う。
/** This class provides the learning algorithm.
 */

struct CorrectMove
{
  CorrectMove(Board board, list<BitBoard> candList, BitBoard pos){
    this->board = board;
    this->candList = candList;
    correctPos = pos;
  }
  
  Board board;
  list<BitBoard> candList;
  BitBoard correctPos;
};

class Learner
{
 private:
  std::vector<CorrectMove> kyokumen;
  
 public:
  /** This function provides one of the supervised learning algorithm
   * called logistic regression.
   * 
   * First, the program makes a file the name of which is decided by "filename".
   * Then, the learning algorithm runs.
   * If you would like to print the detailed information, set "verbose" as true.
   * Finally, the weight vector gets dumped to the file which the program
   * made at the first step.
   */
  void learn(std::string filename = "weight", bool verbose = false);
  void loadKifu();
};
