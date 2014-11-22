#include <ctime>
#include <boost/random.hpp>
#include <iostream>
#include "pattern.h"
#include "board.h"

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

// デバッグ用
// void printPos(BitBoard pos)
// {
//   pair<int, int> coord = Board::posToXY(pos);
//   std::cout << "x, y = " << coord.first << ", " << coord.second << std::endl;
// }


// パターンによる評価値を返す
// 各特徴のインデックスを３進数を用いて表現している
double Pattern::evalFeature(const Board &board)
{
  double score = 0;
  int index;

  // diag4
  // tx[0] = 0;
  // ty[0] = 3;
  int x = 3;
  int y = 7;
  BitBoard pos = Board::xyToPos(x, y);  
  
  // for(int i = 1; i < 4; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1] - 1;
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 4; j++){
      // index += board.getState(tx[j], ty[j]) + 1;
      index += board.getState(pos) + 1;
      /*
	ここのif文はindex *= 3をindex += ....の前に持っていけば消せる。
	分岐予測ミスの削減により速くなる？？
      */
      if(j != 3){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }

    score += weight[DIAG4_OFFSET + index];
    
    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    
    // for(int j = 0; j < 4; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // diag5
  x = 4;
  y = 7;
  pos = Board::xyToPos(x, y);  
  // for(int i = 1; i < 5; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1] - 1;
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 5; j++){
      index += board.getState(pos) + 1;
      if(j != 4){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }
    
    score += weight[DIAG5_OFFSET + index];

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 5; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // diag6
  // tx[0] = 0;
  // ty[0] = 5;
  x = 5;
  y = 7;
  pos = Board::xyToPos(x, y);
  // for(int i = 1; i < 6; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1] - 1;
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 6; j++){
      index += board.getState(pos) + 1;
      if(j != 5){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }
    score += weight[DIAG6_OFFSET + index];

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 6; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // diag7
  // tx[0] = 0;
  // ty[0] = 6;
  x = 6;
  y = 7;
  pos = Board::xyToPos(x, y);  
  // for(int i = 1; i < 7; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1] - 1;
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 7; j++){
      index += board.getState(pos) + 1;
      if(j != 6){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }
    
    score += weight[DIAG7_OFFSET + index];

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 7; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // diag8
  // tx[0] = 0;
  // ty[0] = 7;
  x = 7;
  y = 7;
  pos = Board::xyToPos(x, y);  
  // for(int i = 1; i < 8; i++){    
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1] - 1;
  // }
  for(int i = 0; i < NUM_SIMPLE_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(pos) + 1;
      if(j != 7){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }
    
    score += weight[DIAG8_OFFSET + index];

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 8; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // edge2x
  // tx[0] = 0;
  // ty[0] = 0;
  x = 0;
  y = 7;
  int xx1 = 1;
  int xy1 = 6;
  int xx2 = 1;
  int xy2 = 1; 
  pos = Board::xyToPos(x, y);  
  // for(int i = 1; i < 10; i++){
  //   if(i == 8){
  //     tx[i] = 1;
  //     ty[i] = 1;
  //   }else if(i == 9){
  //     tx[i] = 6;
  //     ty[i] = 1;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(pos) + 1;
      if(j != 9){
	index *= 3;
      }
      if(j < 7){
	pos = Board::transfer(pos, DIRS[i * 2 + 1]);
      }else if(j == 7){
	pos = Board::xyToPos(xx1, xy1);
      }else if(j == 8){
	pos = Board::xyToPos(xx2, xy2);
      }
    }
    
    score += weight[EDGE2X_OFFSET + index];

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);

    ox = xx1;
    oy = xy1;
    xx1 = BOARD_SIZE - 1 - oy;
    xy1 = ox;

    ox = xx2;
    oy = xy2;
    xx2 = BOARD_SIZE - 1 - oy;
    xy2 = ox;
    // for(int j = 0; j < 10; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // corner
  // tx[0] = 0;
  // ty[0] = 0;
  int x0 = 0;
  int y0 = 7;
  int x1 = 1;
  int y1 = 7;
  int x2 = 2;
  int y2 = 7;
  pos = Board::xyToPos(x0, y0);  
  // for(int i = 1; i < 9; i++){
  //   if(i % 3 == 0){
  //     tx[i] = 0;
  //     ty[i] = ty[i - 1] + 1;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 9; j++){
      index += board.getState(pos) + 1;
      if(j != 8){
	index *= 3;
      }
      if(j == 2){
	pos = Board::xyToPos(x1, y1);
      }else if(j == 5){
	pos = Board::xyToPos(x2, y2);
      }else{
	pos = Board::transfer(pos, DIRS[i * 2 + 1]);
      }
    }
    
    score += weight[CORNER_OFFSET + index];

    int ox = x0;
    int oy = y0;
    x0 = BOARD_SIZE - 1 - oy;
    y0 = ox;
    pos = Board::xyToPos(x0, y0);

    ox = x1;
    oy = y1;
    x1 = BOARD_SIZE - 1 - oy;
    y1 = ox;

    ox = x2;
    oy = y2;
    x2 = BOARD_SIZE - 1 - oy;
    y2 = ox;
    // for(int j = 0; j < 9; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // rectangle
  // tx[0] = 0;
  // ty[0] = 0;
  x0 = 0;
  y0 = 7;
  x1 = 1;
  y1 = 7;
  pos = Board::xyToPos(x0, y0);
  // for(int i = 1; i < 10; i++){
  //   if(i % 5 == 0){
  //     tx[i] = 0;
  //     ty[i] = ty[i - 1] + 1;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(pos) + 1;
      if(j != 9){
	index *= 3;
      }
      if(j == 4){
	pos = Board::xyToPos(x1, y1);
      }else{
	pos = Board::transfer(pos, DIRS[i * 2 + 1]);
      }
    }
    
    score += weight[RECTAN_OFFSET + index];

    int ox = x0;
    int oy = y0;
    x0 = BOARD_SIZE - 1 - oy;
    y0 = ox;
    pos = Board::xyToPos(x0, y0);

    ox = x1;
    oy = y1;
    x1 = BOARD_SIZE - 1 - oy;
    y1 = ox;
    // for(int j = 0; j < 10; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // rectangle(mirror)
  // tx[0] = 0;
  // ty[0] = 0;
  x0 = 0;
  y0 = 7;
  x1 = 0;
  y1 = 6;
  pos = Board::xyToPos(x0, y0);
  // for(int i = 1; i < 10; i++){
  //   if(i % 5 == 0){
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1];
  //     ty[i] = ty[i - 1] + 1;
  //   }
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(pos) + 1;
      if(j != 9){
	index *= 3;
      }

      if(j == 4){
	pos = Board::xyToPos(x1, y1);
      }else{
	// (i * 2 + 3) % 8 == (i * 2 + 3) & 7 になる？？
	pos = Board::transfer(pos, DIRS[(i * 2 + 3) % 8]);
      }
    }
    
    score += weight[RECTAN_OFFSET + index];

    int ox = x0;
    int oy = y0;
    x0 = BOARD_SIZE - 1 - oy;
    y0 = ox;
    pos = Board::xyToPos(x0, y0);

    ox = x1;
    oy = y1;
    x1 = BOARD_SIZE - 1 - oy;
    y1 = ox;
    // for(int j = 0; j < 10; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // horizon2
  // tx[0] = 0;
  // ty[0] = 1;
  x = 1;
  y = 7;
  pos = Board::xyToPos(x, y);
  // for(int i = 1; i < 8; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1];
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(pos) + 1;
      if(j != 7){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2 + 1]);
    }
    score += weight[HOR2_OFFSET + index];

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 8; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // horizon3
  // tx[0] = 0;
  // ty[0] = 2;
  x = 2;
  y = 7;
  pos = Board::xyToPos(x, y);
  // for(int i = 1; i < 8; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1];
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(pos) + 1;
      if(j != 7){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2 + 1]);
    }
    
    score += weight[HOR3_OFFSET + index];

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 8; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // horizon4
  // tx[0] = 0;
  // ty[0] = 3;
  x = 3;
  y = 7;
  pos = Board::xyToPos(x, y);
  // for(int i = 1; i < 8; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1];
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(pos) + 1;
      if(j != 7){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2 + 1]);
    }
    
    score += weight[HOR4_OFFSET + index];

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 8; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  return MAGNITUDE * (board.getTurn() == BLACK ? score : -score);
}

// 盤面に現れる特徴のインデックスを取得
void Pattern::extractFeatureIndices(const Board &board, std::vector<int> &indices)
{
  int index;

  // diag4
  // tx[0] = 0;
  // ty[0] = 3;
  int x = 3;
  int y = 7;
  BitBoard pos = Board::xyToPos(x, y);  
  // for(int i = 1; i < 10; i++){
  //   if(i >= 4){
  //     tx[i] = 0;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1] - 1;
  //   }
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 4; j++){
      index += board.getState(pos) + 1;
      if(j != 3){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }
    
    indices.push_back(DIAG4_OFFSET + index);

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    
    // for(int j = 0; j < 4; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // diag5
  x = 4;
  y = 7;
  pos = Board::xyToPos(x, y);  
  // for(int i = 1; i < 5; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1] - 1;
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 5; j++){
      index += board.getState(pos) + 1;
      if(j != 4){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }

    indices.push_back(DIAG5_OFFSET + index);

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 5; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // diag6
  // tx[0] = 0;
  // ty[0] = 5;
  x = 5;
  y = 7;
  pos = Board::xyToPos(x, y);
  // for(int i = 1; i < 6; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1] - 1;
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 6; j++){
      index += board.getState(pos) + 1;
      if(j != 5){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }

    indices.push_back(DIAG6_OFFSET + index);

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 6; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // diag7
  // tx[0] = 0;
  // ty[0] = 6;
  x = 6;
  y = 7;
  pos = Board::xyToPos(x, y);  
  // for(int i = 1; i < 7; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1] - 1;
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 7; j++){
      index += board.getState(pos) + 1;
      if(j != 6){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }

    indices.push_back(DIAG7_OFFSET + index);

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 7; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // diag8
  // tx[0] = 0;
  // ty[0] = 7;
  x = 7;
  y = 7;
  pos = Board::xyToPos(x, y);  
  // for(int i = 1; i < 8; i++){    
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1] - 1;
  // }
  for(int i = 0; i < NUM_SIMPLE_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(pos) + 1;
      if(j != 7){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2]);
    }

    indices.push_back(DIAG8_OFFSET + index);

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 8; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // edge2x
  // tx[0] = 0;
  // ty[0] = 0;
  x = 0;
  y = 7;
  int xx1 = 1;
  int xy1 = 6;
  int xx2 = 1;
  int xy2 = 1; 
  pos = Board::xyToPos(x, y);  
  // for(int i = 1; i < 10; i++){
  //   if(i == 8){
  //     tx[i] = 1;
  //     ty[i] = 1;
  //   }else if(i == 9){
  //     tx[i] = 6;
  //     ty[i] = 1;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(pos) + 1;
      if(j != 9){
	index *= 3;
      }
      if(j < 7){
	pos = Board::transfer(pos, DIRS[i * 2 + 1]);
      }else if(j == 7){
	pos = Board::xyToPos(xx1, xy1);
      }else if(j == 8){
	pos = Board::xyToPos(xx2, xy2);
      }
    }

    indices.push_back(EDGE2X_OFFSET + index);

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);

    ox = xx1;
    oy = xy1;
    xx1 = BOARD_SIZE - 1 - oy;
    xy1 = ox;

    ox = xx2;
    oy = xy2;
    xx2 = BOARD_SIZE - 1 - oy;
    xy2 = ox;
    // for(int j = 0; j < 10; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // corner
  // tx[0] = 0;
  // ty[0] = 0;
  int x0 = 0;
  int y0 = 7;
  int x1 = 1;
  int y1 = 7;
  int x2 = 2;
  int y2 = 7;
  pos = Board::xyToPos(x0, y0);  
  // for(int i = 1; i < 9; i++){
  //   if(i % 3 == 0){
  //     tx[i] = 0;
  //     ty[i] = ty[i - 1] + 1;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 9; j++){
      index += board.getState(pos) + 1;
      if(j != 8){
	index *= 3;
      }
      if(j == 2){
	pos = Board::xyToPos(x1, y1);
      }else if(j == 5){
	pos = Board::xyToPos(x2, y2);
      }else{
	pos = Board::transfer(pos, DIRS[i * 2 + 1]);
      }
    }

    indices.push_back(CORNER_OFFSET + index);

    int ox = x0;
    int oy = y0;
    x0 = BOARD_SIZE - 1 - oy;
    y0 = ox;
    pos = Board::xyToPos(x0, y0);

    ox = x1;
    oy = y1;
    x1 = BOARD_SIZE - 1 - oy;
    y1 = ox;

    ox = x2;
    oy = y2;
    x2 = BOARD_SIZE - 1 - oy;
    y2 = ox;
    // for(int j = 0; j < 9; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // rectangle
  // tx[0] = 0;
  // ty[0] = 0;
  x0 = 0;
  y0 = 7;
  x1 = 1;
  y1 = 7;
  pos = Board::xyToPos(x0, y0);
  // for(int i = 1; i < 10; i++){
  //   if(i % 5 == 0){
  //     tx[i] = 0;
  //     ty[i] = ty[i - 1] + 1;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(pos) + 1;
      if(j != 9){
	index *= 3;
      }
      if(j == 4){
	pos = Board::xyToPos(x1, y1);
      }else{
	pos = Board::transfer(pos, DIRS[i * 2 + 1]);
      }
    }

    indices.push_back(RECTAN_OFFSET + index);

    int ox = x0;
    int oy = y0;
    x0 = BOARD_SIZE - 1 - oy;
    y0 = ox;
    pos = Board::xyToPos(x0, y0);

    ox = x1;
    oy = y1;
    x1 = BOARD_SIZE - 1 - oy;
    y1 = ox;
    // for(int j = 0; j < 10; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // rectangle(mirror)
  // tx[0] = 0;
  // ty[0] = 0;
  x0 = 0;
  y0 = 7;
  x1 = 0;
  y1 = 6;
  pos = Board::xyToPos(x0, y0);
  // for(int i = 1; i < 10; i++){
  //   if(i % 5 == 0){
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1];
  //     ty[i] = ty[i - 1] + 1;
  //   }
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 10; j++){
      index += board.getState(pos) + 1;
      if(j != 9){
	index *= 3;
      }

      if(j == 4){
	pos = Board::xyToPos(x1, y1);
      }else{
	// (i * 2 + 3) % 8 == (i * 2 + 3) & 7 になる？？
	pos = Board::transfer(pos, DIRS[(i * 2 + 3) % 8]);
      }
    }

    indices.push_back(RECTAN_OFFSET + index);

    int ox = x0;
    int oy = y0;
    x0 = BOARD_SIZE - 1 - oy;
    y0 = ox;
    pos = Board::xyToPos(x0, y0);

    ox = x1;
    oy = y1;
    x1 = BOARD_SIZE - 1 - oy;
    y1 = ox;
    // for(int j = 0; j < 10; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }
  
  // horizon2
  // tx[0] = 0;
  // ty[0] = 1;
  x = 1;
  y = 7;
  pos = Board::xyToPos(x, y);
  // for(int i = 1; i < 8; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1];
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(pos) + 1;
      if(j != 7){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2 + 1]);
    }

    indices.push_back(HOR2_OFFSET + index);

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 8; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // horizon3
  // tx[0] = 0;
  // ty[0] = 2;
  x = 2;
  y = 7;
  pos = Board::xyToPos(x, y);
  // for(int i = 1; i < 8; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1];
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(pos) + 1;
      if(j != 7){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2 + 1]);
    }

    indices.push_back(HOR3_OFFSET + index);

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 8; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // horizon4
  // tx[0] = 0;
  // ty[0] = 3;
  x = 3;
  y = 7;
  pos = Board::xyToPos(x, y);
  // for(int i = 1; i < 8; i++){
  //   tx[i] = tx[i - 1] + 1;
  //   ty[i] = ty[i - 1];
  // }
  for(int i = 0; i < NUM_SYMMETRY; i++){
    index = 0;
    for(int j = 0; j < 8; j++){
      index += board.getState(pos) + 1;
      if(j != 7){
	index *= 3;
      }
      pos = Board::transfer(pos, DIRS[i * 2 + 1]);
    }

    indices.push_back(HOR4_OFFSET + index);

    int ox = x;
    int oy = y;
    x = BOARD_SIZE - 1 - oy;
    y = ox;
    pos = Board::xyToPos(x, y);
    // for(int j = 0; j < 8; j++){
    //   ox = tx[j];
    //   oy = ty[j];
    //   tx[j] = BOARD_SIZE - 1 - oy;
    //   ty[j] = ox;
    // }
  }

  // // diag5
  // tx[0] = 0;
  // ty[0] = 4;
  // for(int i = 1; i < 10; i++){
  //   if(i >= 5){
  //     tx[i] = 0;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1] - 1;
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 5; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 4){
  // 	index *= 3;
  //     }
  //   }
    
  //   indices.push_back(DIAG5_OFFSET + index);
  //   for(int j = 0; j < 5; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }
  
  // // diag6
  // tx[0] = 0;
  // ty[0] = 5;
  // for(int i = 1; i < 10; i++){
  //   if(i >= 6){
  //     tx[i] = 0;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1] - 1;
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 6; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 5){
  // 	index *= 3;
  //     }
  //   }
    
  //   indices.push_back(DIAG6_OFFSET + index);
  //   for(int j = 0; j < 6; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }
  
  // // diag7
  // tx[0] = 0;
  // ty[0] = 6;
  // for(int i = 1; i < 10; i++){
  //   if(i >= 7){
  //     tx[i] = 0;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1] - 1;
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 7; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 6){
  // 	index *= 3;
  //     }
  //   }
    
  //   indices.push_back(DIAG7_OFFSET + index);
  //   for(int j = 0; j < 7; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }

  // // diag8
  // tx[0] = 0;
  // ty[0] = 7;
  // for(int i = 1; i < 10; i++){
  //   if(i >= 8){
  //     tx[i] = 0;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1] - 1;
  //   }
  // }
  // for(int i = 0; i < NUM_SIMPLE_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 8; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 7){
  // 	index *= 3;
  //     }
  //   }
    
  //   indices.push_back(DIAG8_OFFSET + index);
  //   for(int j = 0; j < 8; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }

  // // edge2x
  // tx[0] = 0;
  // ty[0] = 0;
  // for(int i = 1; i < 10; i++){
  //   if(i == 8){
  //     tx[i] = 1;
  //     ty[i] = 1;
  //   }else if(i == 9){
  //     tx[i] = 6;
  //     ty[i] = 1;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 10; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 9){
  // 	index *= 3;
  //     }
  //   }
    
  //   indices.push_back(EDGE2X_OFFSET + index);
  //   for(int j = 0; j < 10; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }
  
  // // corner
  // tx[0] = 0;
  // ty[0] = 0;
  // for(int i = 1; i < 10; i++){
  //   if(i == 9){
  //     tx[i] = 0;
  //     ty[i] = 0;
  //   }else if(i % 3 == 0){
  //     tx[i] = 0;
  //     ty[i] = ty[i - 1] + 1;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 9; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 8){
  // 	index *= 3;
  //     }
  //   }

  //   indices.push_back(CORNER_OFFSET + index);
  //   for(int j = 0; j < 9; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }

  // // rectangle
  // tx[0] = 0;
  // ty[0] = 0;
  // for(int i = 1; i < 10; i++){
  //   if(i % 5 == 0){
  //     tx[i] = 0;
  //     ty[i] = ty[i - 1] + 1;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 10; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 9){
  // 	index *= 3;
  //     }
  //   }
    
  //   indices.push_back(RECTAN_OFFSET + index);
  //   for(int j = 0; j < 10; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }

  // // rectangle(mirror)
  // tx[0] = 0;
  // ty[0] = 0;
  // for(int i = 1; i < 10; i++){
  //   if(i % 5 == 0){
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1];
  //     ty[i] = ty[i - 1] + 1;
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 10; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 9){
  // 	index *= 3;
  //     }
  //   }
    
  //   indices.push_back(RECTAN_OFFSET + index);
  //   for(int j = 0; j < 10; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }

  // // horizon2
  // tx[0] = 0;
  // ty[0] = 1;
  // for(int i = 1; i < 10; i++){
  //   if(i >= 8){
  //     tx[i] = 0;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 8; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 7){
  // 	index *= 3;
  //     }
  //   }
  //   indices.push_back(HOR2_OFFSET + index);
  //   for(int j = 0; j < 8; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }

  // // horizon3
  // tx[0] = 0;
  // ty[0] = 2;
  // for(int i = 1; i < 10; i++){
  //   if(i >= 8){
  //     tx[i] = 0;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 8; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 7){
  // 	index *= 3;
  //     }
  //   }
  //   indices.push_back(HOR3_OFFSET + index);
  //   for(int j = 0; j < 8; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }

  // // horizon4
  // tx[0] = 0;
  // ty[0] = 3;
  // for(int i = 1; i < 10; i++){
  //   if(i >= 8){
  //     tx[i] = 0;
  //     ty[i] = 0;
  //   }else{
  //     tx[i] = tx[i - 1] + 1;
  //     ty[i] = ty[i - 1];
  //   }
  // }
  // for(int i = 0; i < NUM_SYMMETRY; i++){
  //   index = 0;
  //   for(int j = 0; j < 8; j++){
  //     index += board.getState(pos) + 1;
  //     if(j != 7){
  // 	index *= 3;
  //     }
  //   }
  //   indices.push_back(HOR4_OFFSET + index);
  //   for(int j = 0; j < 8; j++){
  //     ox = tx[j];
  //     oy = ty[j];
  //     tx[j] = BOARD_SIZE - 1 - oy;
  //     ty[j] = ox;
  //   }
  // }

}

// void Pattern::normalizeWeight(int norm)
// {
//   long long int nsum = 0;
//   for(int i = 0; i < FEATURE_NUM; i++){
//     double weight = getWeight(i);
//     nsum += (long long int)(weight * weight);
//   }
//   std::cout << nsum << std::endl;
//   nsum = (long long int)sqrt(nsum);
//   for(int i = 0; i < FEATURE_NUM; i++){
//     double weight = getWeight(i);
//     setWeight(i, (int)(weight * norm / nsum));
//   }
// }