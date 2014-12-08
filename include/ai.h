#pragma once

#include <vector>
#include <stdio.h>
#include <boost/unordered_map.hpp>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include "board.h"
#include "jouseki.h"
#include "pattern.h"

struct BoardState;
typedef boost::unordered_map<Board, BoardState> BoardHash;

using std::begin;
using std::end;
using std::next;

const int INF = INT_MAX;
const int DEFAULT_SEARCH_TIME = 1;
const int MAX_VALUE = 50000;
const double DELTA = 0.000001;  //微小量
/* const double SCALE = 1.5; */

const int THRESH_MOVE_ORDERING_DEPTH = 4;
const int HASH_REGISTER_DEPTH = 5;

//ProbCutのマージン
/* const double MARGIN3 = 7.47179 * SCALE; */
/* const double MARGIN4 = 6.13008 * SCALE; */
/* const double MARGIN5 = 8.9405 * SCALE; */
/* const double MARGIN6 = 7.00334 * SCALE; */
/* const double MARGIN7 = 9.27589 * SCALE; */
/* const double MARGIN8 = 6.1871 * SCALE; */
/* const double MARGIN9 = 7.58629 * SCALE; */
/* const double MARGIN10 = 6.56639 * SCALE; */

/* const int GRANULARITY = 7; */


/** When you search the game tree by Search::sc_negamax(),
 * the type of the returned value is this structure.
 */
struct MoveInfo
{
 public:
  BitBoard pos;
  double score;
MoveInfo() : pos(0), score(0){}
};

struct DetailedMoveInfo : public MoveInfo
{
 public:
  LightBoard board;
};

/** You control the search time
 * using this structure.
 */
struct SearchTime
{
 public:
  double start;
  double searchTime;
SearchTime() : start(0), searchTime(DEFAULT_SEARCH_TIME){}
};

/* When you use the hash, the type of the stored data */
/*  is this structure.  */
struct BoardState
{
public:
  int turn;  //その盤面での手番
  int depth;  //その盤面に辿り着いた時の残り探索深さ
  BitBoard pos;
  double score;  //その盤面の評価値
BoardState() : turn(0), depth(-1), pos(0), score(0){}
  BoardState(int turn, int depth, BitBoard pos, double score){
    this->turn = turn;
    this->depth = depth;
    this->pos = pos;
    this->score = score;
  }
};

/** This class provides the funtction to 
 * search the game tree.
 */
class AI
{ 
 public:
  /** Read the jouseki data and store it to the variable "jouseki". */
  AI();
  void setTime(double val)
  {
    st.searchTime = val;
  }
  void init()
  {
    bh.clear();
    jouseki.randJousekiList();
  }
    
  /** This function searches the game tree.
   * If "is_ordering" is true, you search the child node preferentially 
   * which seems to return the best value as a result of the shallow search. 
   * 
   * If "is_prob" is true, the ProbCut algorithm runs.
   * Probcut algorithm is the way to prune the child nodes whose values are
   * extremely out of the range of the alpha-beta window.
   * 
   * If you set a position to "pcx", "pcy", the program preferentially searches
   * the child node which you can get by putting a stone on "pcx", "pcy".
   * This is available when "is_ordering" is false.
   */
  MoveInfo negascout(Board &board, double alpha, double beta, int depth,
		     bool nullWindowSearch = false);
  MoveInfo minimax(Board &board, int depth);
		     /* bool isOrdering = true, bool isProb = true, */
		     /* int pcx = 0, int pcy = 0); */

  DetailedMoveInfo detailedNegascout(Board &board, double alpha, double beta, int depth, const Pattern& lnPt);
  
  //MoveInfo sc_jamboree(const spBan ban, int turn, int alpha, int beta, int depth, bool is_root = false);
  /** This function is intended to be used in actual game play.
   * If "is_itr" is true, the iteretive deepning algorithm runs.
   */
  void search(Board &board, int depth);
  /** This function calculates the leaf value.
   * If the winner is decided, this function adds
   * or describes 5000 points to the usual return value.
   */
  MoveInfo eval(const Board &board);
  DetailedMoveInfo detailedEval(const LightBoard &board, const Pattern &lnPt);
  
 private:
  Jouseki jouseki;
  Pattern pt;
  SearchTime st;
  BoardHash bh;
  int numSearchNode;
};

/** This class is used when you run the learning.
 */
/* class Search2 : public Search */
/* { */
/*  public: */
/*   /\** This function calculates the leaf value. */
/*    * The difference between this functions and the counter part in the "Search" class */
/*    * is whether the function adds the winner's bonus or not. */
/*    *\/ */
/*   MoveInfo eval(const spBan ban, int turn); */
/*   /\** This function searches the game tree. */
/*    * The difference between this functions and the counter part in the "Search" class */
/*    * is whether the function uses the hash or not. */
/*    *\/ */
/*   MoveInfo sc_negamax(const spBan ban, int turn, double alpha, double beta, int depth, */
/* 		   bool is_ordering = true, bool is_prob = true, */
/* 		   int pcx = 0, int pcy = 0); */
/* }; */

