#include "learner.h"
#include "random.h"
#include "search.h"
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>


//棋譜ファイルを読んで局面を変数kyokumenに突っ込んでいき、最後にrandomize
void Learner::loadKifu()
{
  std::ifstream ifs;
  std::string line;
  char f_name[32];
  bool kifu_kind = 0;
  int offset_self = 0;

  for(int i = 0; i < LOAD_KIFU_NUM; i++){
    if(kifu_kind == 0){
      sprintf(f_name, "kifu/pro/kifu%d", i);
      ifs.open(f_name);
      if(!ifs){
	kifu_kind = 1;
	offset_self = i;
      }
    }
    if(kifu_kind == 1){
      sprintf(f_name, "kifu/edax/kifu%d", i - offset_self);
      ifs.open(f_name);
      if(!ifs){
	std::cerr << "Too few training data!" << std::endl;
	exit(1);
      }
    }

    
    Board board;
    int count = 0;
    int tx, ty;
    while(getline(ifs, line)){
      if(line == "\n") break;
      std::stringstream buf(line);
      buf >> tx >> ty;
      // 人間視点の手をコンピュータ視点に変換
      tx--;
      ty--;
      BitBoard pos = Board::xyToPos(tx, ty);
      //最初の数手はいらない
      if(count < THRESH_NUM_CUTOFF_MOVE){
	//banを一手ずつ進めていく
	board.putStone(pos, true);
	count++;
	continue;
      }
      //実体を作る
      Board tboard = board;
      std::cout << f_name << std::endl;
      tboard.display();
      // tboard.set_turn(turn);
      //正解の指し手を保存していく
      // tboard.set_correct(tx, ty);
      kyokumen.push_back(CorrectMove(tboard, tboard.getCl(), pos));
      //banを一手ずつ進めていく
      //パスなら手順を入れ替えてやり直し
      if(!board.putStone(pos, true)){
	assert(board.isEnd());
	kyokumen.pop_back();
      }
    }
    //最後の局面は手が決まってしまっており、学習に利用できないので破棄
    kyokumen.pop_back();
    ifs.close();
  }
  
  //randomize
  Random rnd;
  random_shuffle(kyokumen.begin(), kyokumen.end(), rnd);

  std::cout << "Learning data randomized!" << std::endl;
  std::cout << "Loaded kyokumen num:" << kyokumen.size() << std::endl;
}

//Bonanza method
void Learner::learn(std::string filename, bool verbose)
{
  list<int> cvalidIndices, ovalidIndices;  //特徴ベクトルが0ではないindices
  AI ai;
  Pattern pt;
  //棋譜ファイルを読み込む
  loadKifu();

  //重みを初期化
  pt.initWeight();

  std::cout << "Weight initialization complete!" << std::endl;

  for(int h = 0; h < REPEAT_NUM; h++){
    for(vector<CorrectMove>::iterator i = begin(kyokumen);
	i != end(kyokumen); i++){
      std::cout << "repeat:" << h << ", kyokumen:"
		<< distance(begin(kyokumen), i) << std::endl;
      
      if(verbose){
	i->board.display();
      }
      int turn = i->board.getTurn();
      BitBoard cpos = i->correctPos;
      std::string tlabel = (turn == BLACK ? "BLACK" : "WHITE");
      if(verbose){
	std::cout << "turn:" << tlabel << std::endl;
	pair<int, int> coord = Board::posToXY(cpos);
	std::cout << "correct pos:" << coord.first << ", "
		  << coord.second << std::endl;
      }
      BitBoard revPattern;
      revPattern = i->board.putStone(cpos);      
      //棋譜の手から深さ3or5の探索を行った局面の評価値を取得する
      DetailedMoveInfo cv;  //棋譜の手、評価値、末端局面を保持する
      cv = ai.detailedNegascout(i->board, -INF, INF,
				h < (REPEAT_NUM >> 1) ? 3 : 5, pt);
      cv.score *= -1;
      cv.pos = cpos;
      i->board.undo(cpos, revPattern);
      // double cmbsValue = -sc.eval(cv.ban, -turn).value;      
      if(verbose){
	// double cmbsValue = ai.detailedEval(cv.board, pt).score;
	std::cout << "correct value:" << cv.score << std::endl;
	// std::cout << "must be same:" << cmbsValue << std::endl;
	// assert(cv.score == cmbsValue || cv.score == -cmbsValue);

	std::cout << "correct terminal situation:" << std::endl;
	cv.board.display();
      }
      //棋譜の手から探索を行った末端局面の特徴ベクトルを抽出
      cvalidIndices.clear();
      // bkCvalidIndices.clear();
      pt.extractFeatureIndices(cv.board, cvalidIndices);
      // bkCvalidIndices.assign(cvalidIndices.begin(), cvalidIndices.end());
      
      for(list<BitBoard>::const_iterator j = begin(i->candList);
	  j != end(i->candList); j++){
	//棋譜の手なら飛ばす
	// if(cv.x == j->first && cv.y == j->second){
	if(cv.pos == *j){
	  continue;
	}
	if(i->board.canPut(*j)){
	  BitBoard revPattern;
	  revPattern = i->board.putStone(*j);
	  if(verbose){
	    pair<int, int> coord = Board::posToXY(*j);
	    std::cout << "other pos:" << coord.first << ", "
		      << coord.second << std::endl;
	  }
	  //ここから深さ５の探索を行う
	  DetailedMoveInfo ov;  //棋譜の手以外の情報を保持する
	  ov = ai.detailedNegascout(i->board, -INF, INF,
				    h < (REPEAT_NUM >> 1) ? 3 : 5, pt);
	  ov.score *= -1;
	  ov.pos = *j;
	  i->board.undo(*j, revPattern);
	  if(verbose){
	    // double ombsValue = ai.detailedEval(ov.board, pt).score;
	    std::cout << "other value:" << ov.score << std::endl;
	    // std::cout << "must be same:" << ombsValue << std::endl;
	    // assert(ov.score == ombsValue || ov.score == -ombsValue);
	  }

	  // 棋譜の手の方がスコアが低い場合、これを是正する
	  if(cv.score <= ov.score){	  
	    /*
	      if(verbose){
	      std::cout << "correct terminal situation:" << std::endl;
	      ov.ban->show_ban();
	      }
	    */
	    double logistic;
	    logistic = exp(-ACCURACY * (ov.score - cv.score));
	    //その他の合法手の特徴ベクトルを抽出
	    ovalidIndices.clear();
	    pt.extractFeatureIndices(ov.board, ovalidIndices);
	  
	    //cvalidIndicesの値を復帰
	    // cvalidIndices.clear();
	    // cvalidIndices.assign(bkCvalidIndices.begin(), bkCvalidIndices.end());

	    // bool erased = false;
	    //cvalidIndicesとovalidIndicesで共通に現れる特徴はあらかじめはじいておく
	    for(list<int>::iterator citr = cvalidIndices.begin(); citr != cvalidIndices.end(); citr++){
	      for(list<int>::iterator oitr = ovalidIndices.begin(); oitr != ovalidIndices.end(); oitr++){
		if(*citr == *oitr){
		  citr = cvalidIndices.erase(citr);
		  ovalidIndices.erase(oitr);
		  citr--;
		  break;
		}		
	      }
	    }

	    // for(int fnum = 0; fnum < (signed int)cvalidIndices.size(); fnum++){
	    for(list<int>::iterator itr = begin(cvalidIndices);
		itr != end(cvalidIndices); itr++){
	      const double cw = pt.getWeight(*itr);
	      //黒なら重みを大きくし、白なら小さくする
	      assert((1 + logistic) * (1 + logistic) != 0);	    
	      if(turn == BLACK){
		pt.setWeight(*itr, cw + LEARNING_RATE * ACCURACY * logistic / ((1 + logistic) * (1 + logistic)));
	      }else{
		pt.setWeight(*itr, cw - LEARNING_RATE * ACCURACY * logistic / ((1 + logistic) * (1 + logistic)));
	      }
	      // cout << pt.getWeight(cvalidIndices.at(fnum)) << endl;
	      // cout << LEARNING_RATE * ACCURACY * logistic / ((1 + logistic) * (1 + logistic)) << endl;
	    }

	    // for(int fnum = 0; fnum < (signed int)ovalidIndices.size(); fnum++){
	    for(list<int>::iterator itr = begin(ovalidIndices);
		itr != end(ovalidIndices); itr++){
	      const double ow = pt.getWeight(*itr);
	      //黒なら重みを小さくし、白なら大きくする
	      assert((1 + logistic) * (1 + logistic) != 0);
	      if(turn == BLACK){		    
		pt.setWeight(*itr, ow - LEARNING_RATE * ACCURACY * logistic / ((1 + logistic) * (1 + logistic))); //pow(1 + logistic, 2));
	      }else{
		pt.setWeight(*itr, ow + LEARNING_RATE * ACCURACY * logistic / ((1 + logistic) * (1 + logistic)));
	      }
	      // cout << pt.getWeight(cvalidIndices.at(fnum)) << endl;
	      // cout << LEARNING_RATE * ACCURACY * logistic / ((1 + logistic) * (1 + logistic)) << endl;
	    }
	    cv.score = ai.detailedEval(cv.board, pt).score;
	    if(verbose){
	      std::cout << "new correct value:" << cv.score  << std::endl;
	      std::cout << "new other value:"
			<< ai.detailedEval(ov.board, pt).score << std::endl;
	      std::cout << std::endl;
	    }
	  }else{
	    if(verbose){
	      cout << "skip" << endl;
	    }
	  }
	}
      }
    }

    //L2正則化
    for(int i = 0; i < FEATURE_NUM; i++){
      double w = pt.getWeight(i);
      pt.setWeight(i, w - LEARNING_RATE * GAMMA * 2 * w);
    }
  }
  std::cout << "Learning complete!" << std::endl;
  //ファイル書き出し
  std::ofstream ofs;
  ofs.open(filename.c_str());
  for(int i = 0; i < FEATURE_NUM; i++){
    ofs << pt.getWeight(i) << std::endl;
  }

  kyokumen.clear();
}
