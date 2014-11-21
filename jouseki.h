#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "board.h"

using std::string;
using std::begin;
using std::end;
using std::vector;
using std::list;
using std::pair;
using std::cout;
using std::endl;

const string DEFAULT_JOUSEKI_FILENAME = "jouseki";
/* const int NUM_SYMMETRY = 4; */

/** This class provides the functions to use the opening database.
 */
class Jouseki
{ 
 public:
  void readJousekiFile(string filename = DEFAULT_JOUSEKI_FILENAME);
  bool useJouseki(Board &board);
  /** This function randomize the list of opening database.
   */  
 private:
  vector<list<BitBoard> > jousekiList;
  void randJousekiList();
};

