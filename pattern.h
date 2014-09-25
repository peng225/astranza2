#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include "board.h"

const int NUM_SIMPLE_SYMMETRY = 2;
const int NUM_SYMMETRY = 4;

// each feature's number
const int MAX_P10 = 59049;
const int MAX_P9 = 19683;
const int MAX_P8 = 6561;
const int MAX_P7 = 2187;
const int MAX_P6 = 729;
const int MAX_P5 = 243;
const int MAX_P4 = 81;

// offset
const int DIAG4_OFFSET = 1;
const int DIAG5_OFFSET = DIAG4_OFFSET + MAX_P4;
const int DIAG6_OFFSET = DIAG5_OFFSET + MAX_P5;
const int DIAG7_OFFSET = DIAG6_OFFSET + MAX_P6;
const int DIAG8_OFFSET = DIAG7_OFFSET + MAX_P7;
const int EDGE2X_OFFSET = DIAG8_OFFSET + MAX_P8;
const int CORNER_OFFSET = EDGE2X_OFFSET + MAX_P10;
const int RECTAN_OFFSET = CORNER_OFFSET + MAX_P9;
const int HOR2_OFFSET = RECTAN_OFFSET + MAX_P10;
const int HOR3_OFFSET = HOR2_OFFSET + MAX_P8;
const int HOR4_OFFSET = HOR3_OFFSET + MAX_P8;

// 最初の+1はpossible_place_numberのため
// possible place numberって今は使ってなくね？
const int FEATURE_NUM = 1 + MAX_P4 + MAX_P5 + MAX_P6 + MAX_P7 + MAX_P8 * 4 + MAX_P9 + MAX_P10 * 2;

class Pattern
{ 
 public:
  Pattern();
  void initWeight();
  void loadWeight(std::string wName);
  int evalFeature(const Board &board);
  void setWeight(int i, double val)
  {
    weight[i] = val;
  }
  double getWeight(int i)
  {
    return weight[i];
  }
  void extractFeatureIndices(const Board &board, std::vector<int> &indices);
  void normalizeWeight(int norm);
 private:
  double weight[FEATURE_NUM]; 
};


