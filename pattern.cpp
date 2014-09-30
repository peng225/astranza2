#include <ctime>
#include <boost/random.hpp>
#include "pattern.h"

// BAN_SIZE == 10で、BOARD_SIZE == 8である。注意せよ。

using namespace boost;

Pattern::Pattern()
{
  std::ifstream ifs("weight");
  std::string line;
  if(!ifs){
    std::cout << "There is no weight file. Initialize weight as 0." << std::endl;
    initWeight();
  }else{
    for(int i = 0; i < FEATURE_NUM; i++){
      getline(ifs, line);
      std::stringstream buf(line);
      buf >> weight[i];
    }
    std::cout << "Loaded weight." << std::endl;
  }
  ifs.close();
}

// 重みを0で初期化
void Pattern::initWeight()
{

  for(int i = 0; i < FEATURE_NUM; i++){
    weight[i] = 0;
  }
  // 自分と相手の置ける場所の差に対する重み
  weight[0] = 1;
}

void Pattern::loadWeight(std::string wName)
{
  std::ifstream ifs(wName.c_str());
  std::string line;
  if(!ifs){
    std::cout << "There is no weight file named " + wName
      + ". Initialize weight as 0." 
	      << std::endl;
    initWeight();
  }else{
    for(int i = 0; i < FEATURE_NUM; i++){
      getline(ifs, line);
      std::stringstream buf(line);
      buf >> weight[i];
    }
    std::cout << "Loaded " + wName + "." << std::endl;
  }
  ifs.close();
}


// パターンによる評価値を返す
// 各特徴のインデックスを３進数を用いて表現している
int Pattern::evalFeature(const Board &board)
{
  double score = 0;
  int index;
  int tx[10], ty[10];
  int ox, oy;

  // diag4
  tx[0] = 0;
  ty[0] = 3;
  for(int i = 1; i < 4; i++){
    tx[i] = tx[i - 1] + 1;
    ty[i] = ty[i - 1] - 1;
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 4; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 3){
	index *= 3;
      }
    }

    score += weight[DIAG4_OFFSET + index];
    for(int j = 0; j < 4; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }
  
  // diag5
  tx[0] = 0;
  ty[0] = 4;
  for(int i = 1; i < 5; i++){
    tx[i] = tx[i - 1] + 1;
    ty[i] = ty[i - 1] - 1;
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 5; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 4){
	index *= 3;
      }
    }
    
    score += weight[DIAG5_OFFSET + index];
    for(int j = 0; j < 5; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }
  
  // diag6
  tx[0] = 0;
  ty[0] = 5;
  for(int i = 1; i < 6; i++){
    tx[i] = tx[i - 1] + 1;
    ty[i] = ty[i - 1] - 1;
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 6; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 5){
	index *= 3;
      }
    }
    score += weight[DIAG6_OFFSET + index];
    for(int j = 0; j < 6; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }
  
  // diag7
  tx[0] = 0;
  ty[0] = 6;
  for(int i = 1; i < 7; i++){
    tx[i] = tx[i - 1] + 1;
    ty[i] = ty[i - 1] - 1;
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 7; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 6){
	index *= 3;
      }
    }
    
    score += weight[DIAG7_OFFSET + index];
    for(int j = 0; j < 7; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // diag8
  tx[0] = 0;
  ty[0] = 7;
  for(int i = 1; i < 8; i++){    
    tx[i] = tx[i - 1] + 1;
    ty[i] = ty[i - 1] - 1;
  }
  for(int i = 0; i < NUM_SIMPLE_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 7){
	index *= 3;
      }
    }
    
    score += weight[DIAG8_OFFSET + index];
    for(int j = 0; j < 8; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }
  
  // edge2x
  tx[0] = 0;
  ty[0] = 0;
  for(int i = 1; i < 10; i++){
    if(i == 8){
      tx[i] = 1;
      ty[i] = 1;
    }else if(i == 9){
      tx[i] = 6;
      ty[i] = 1;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1];
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 9){
	index *= 3;
      }
    }
    
    score += weight[EDGE2X_OFFSET + index];
    for(int j = 0; j < 10; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }
  
  // corner
  tx[0] = 0;
  ty[0] = 0;
  for(int i = 1; i < 9; i++){
    if(i % 3 == 0){
      tx[i] = 0;
      ty[i] = ty[i - 1] + 1;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1];
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 9; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 8){
	index *= 3;
      }
    }
    
    score += weight[CORNER_OFFSET + index];
    for(int j = 0; j < 9; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // rectangle
  tx[0] = 0;
  ty[0] = 0;
  for(int i = 1; i < 10; i++){
    if(i % 5 == 0){
      tx[i] = 0;
      ty[i] = ty[i - 1] + 1;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1];
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 9){
	index *= 3;
      }
    }
    
    score += weight[RECTAN_OFFSET + index];
    for(int j = 0; j < 10; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // rectangle(mirror)
  tx[0] = 0;
  ty[0] = 0;
  for(int i = 1; i < 10; i++){
    if(i % 5 == 0){
      tx[i] = tx[i - 1] + 1;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1];
      ty[i] = ty[i - 1] + 1;
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 9){
	index *= 3;
      }
    }
    
    score += weight[RECTAN_OFFSET + index];
    for(int j = 0; j < 10; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }
  
  // horizon2
  tx[0] = 0;
  ty[0] = 1;
  for(int i = 1; i < 8; i++){
    tx[i] = tx[i - 1] + 1;
    ty[i] = ty[i - 1];
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 7){
	index *= 3;
      }
    }
    score += weight[HOR2_OFFSET + index];
    for(int j = 0; j < 8; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // horizon3
  tx[0] = 0;
  ty[0] = 2;
  for(int i = 1; i < 8; i++){
    tx[i] = tx[i - 1] + 1;
    ty[i] = ty[i - 1];
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 7){
	index *= 3;
      }
    }
    score += weight[HOR3_OFFSET + index];
    for(int j = 0; j < 8; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // horizon4
  tx[0] = 0;
  ty[0] = 3;
  for(int i = 1; i < 8; i++){
    tx[i] = tx[i - 1] + 1;
    ty[i] = ty[i - 1];
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 7){
	index *= 3;
      }
    }
    score += weight[HOR4_OFFSET + index];
    for(int j = 0; j < 8; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  return MAGNITUDE * (board.getTurn() == BLACK ? score : -score);
}

// 盤面に現れる特徴のインデックスを取得
void Pattern::extractFeatureIndices(const Board &board, std::vector<int> &indices)
{
  int index;
  int tx[10], ty[10];
  int ox, oy;

  // possible_place_number
  /*
  double num = board->possible_place_number(BLACK);
  for(int i = 0; i < num; i++){
    indices.push_back(0);
  }
  */

  // diag4
  tx[0] = 0;
  ty[0] = 3;
  for(int i = 1; i < 10; i++){
    if(i >= 4){
      tx[i] = 0;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1] - 1;
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 4; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 3){
	index *= 3;
      }
    }
    
    indices.push_back(DIAG4_OFFSET + index);
    for(int j = 0; j < 4; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // diag5
  tx[0] = 0;
  ty[0] = 4;
  for(int i = 1; i < 10; i++){
    if(i >= 5){
      tx[i] = 0;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1] - 1;
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 5; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 4){
	index *= 3;
      }
    }
    
    indices.push_back(DIAG5_OFFSET + index);
    for(int j = 0; j < 5; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }
  
  // diag6
  tx[0] = 0;
  ty[0] = 5;
  for(int i = 1; i < 10; i++){
    if(i >= 6){
      tx[i] = 0;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1] - 1;
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 6; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 5){
	index *= 3;
      }
    }
    
    indices.push_back(DIAG6_OFFSET + index);
    for(int j = 0; j < 6; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }
  
  // diag7
  tx[0] = 0;
  ty[0] = 6;
  for(int i = 1; i < 10; i++){
    if(i >= 7){
      tx[i] = 0;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1] - 1;
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 7; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 6){
	index *= 3;
      }
    }
    
    indices.push_back(DIAG7_OFFSET + index);
    for(int j = 0; j < 7; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // diag8
  tx[0] = 0;
  ty[0] = 7;
  for(int i = 1; i < 10; i++){
    if(i >= 8){
      tx[i] = 0;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1] - 1;
    }
  }
  for(int i = 0; i < NUM_SIMPLE_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 7){
	index *= 3;
      }
    }
    
    indices.push_back(DIAG8_OFFSET + index);
    for(int j = 0; j < 8; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // edge2x
  tx[0] = 0;
  ty[0] = 0;
  for(int i = 1; i < 10; i++){
    if(i == 8){
      tx[i] = 1;
      ty[i] = 1;
    }else if(i == 9){
      tx[i] = 6;
      ty[i] = 1;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1];
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 9){
	index *= 3;
      }
    }
    
    indices.push_back(EDGE2X_OFFSET + index);
    for(int j = 0; j < 10; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }
  
  // corner
  tx[0] = 0;
  ty[0] = 0;
  for(int i = 1; i < 10; i++){
    if(i == 9){
      tx[i] = 0;
      ty[i] = 0;
    }else if(i % 3 == 0){
      tx[i] = 0;
      ty[i] = ty[i - 1] + 1;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1];
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 9; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 8){
	index *= 3;
      }
    }

    indices.push_back(CORNER_OFFSET + index);
    for(int j = 0; j < 9; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // rectangle
  tx[0] = 0;
  ty[0] = 0;
  for(int i = 1; i < 10; i++){
    if(i % 5 == 0){
      tx[i] = 0;
      ty[i] = ty[i - 1] + 1;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1];
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 9){
	index *= 3;
      }
    }
    
    indices.push_back(RECTAN_OFFSET + index);
    for(int j = 0; j < 10; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // rectangle(mirror)
  tx[0] = 0;
  ty[0] = 0;
  for(int i = 1; i < 10; i++){
    if(i % 5 == 0){
      tx[i] = tx[i - 1] + 1;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1];
      ty[i] = ty[i - 1] + 1;
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 9){
	index *= 3;
      }
    }
    
    indices.push_back(RECTAN_OFFSET + index);
    for(int j = 0; j < 10; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // horizon2
  tx[0] = 0;
  ty[0] = 1;
  for(int i = 1; i < 10; i++){
    if(i >= 8){
      tx[i] = 0;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1];
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 7){
	index *= 3;
      }
    }
    indices.push_back(HOR2_OFFSET + index);
    for(int j = 0; j < 8; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // horizon3
  tx[0] = 0;
  ty[0] = 2;
  for(int i = 1; i < 10; i++){
    if(i >= 8){
      tx[i] = 0;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1];
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 7){
	index *= 3;
      }
    }
    indices.push_back(HOR3_OFFSET + index);
    for(int j = 0; j < 8; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

  // horizon4
  tx[0] = 0;
  ty[0] = 3;
  for(int i = 1; i < 10; i++){
    if(i >= 8){
      tx[i] = 0;
      ty[i] = 0;
    }else{
      tx[i] = tx[i - 1] + 1;
      ty[i] = ty[i - 1];
    }
  }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(tx[j], ty[j]) + 1;
      if(j != 7){
	index *= 3;
      }
    }
    indices.push_back(HOR4_OFFSET + index);
    for(int j = 0; j < 8; j++){
      ox = tx[j];
      oy = ty[j];
      tx[j] = BOARD_SIZE - 1 - oy;
      ty[j] = ox;
    }
  }

}

void Pattern::normalizeWeight(int norm)
{
  long long int nsum = 0;
  for(int i = 0; i < FEATURE_NUM; i++){
    double weight = getWeight(i);
    nsum += (long long int)(weight * weight);
  }
  std::cout << nsum << std::endl;
  nsum = (long long int)sqrt(nsum);
  for(int i = 0; i < FEATURE_NUM; i++){
    double weight = getWeight(i);
    setWeight(i, (int)(weight * norm / nsum));
  }
}
