#include "ai.h"
#include <map>

using std::list;
using std::map;

size_t hash_value(const Board &board)
{
  // 複数の値のハッシュ値を組み合わせてハッシュ値を計算するには、
  // boost::hash_combine を使います。
  size_t h = 0;
  boost::hash_combine(h, board.getBlack());
  boost::hash_combine(h, board.getWhite());
  return h;
}

//private member of Search
//末端評価
MoveInfo AI::eval(const Board &board)
{
  MoveInfo info;
  //勝敗が決まっていたら、max or minの評価値を返す
  if(board.isEnd()){
    State_t winner = board.getWinner();
    if(winner == board.getTurn()){
      info.score = MAX_VALUE + pt.evalFeature(board);
    }else if(winner == SPACE){
      info.score = 0;
    }else{
      info.score = -MAX_VALUE + pt.evalFeature(board);
    }
    return info;
  }
  info.score = pt.evalFeature(board);
  return info;
}

DetailedMoveInfo AI::detailedEval(const LightBoard &board, const Pattern &lnPt)
{
  DetailedMoveInfo info;
  info.score = lnPt.evalFeature(board);
  info.board = board;
  return info;
}


// NegaScout
// 主にプレイ時に使用
MoveInfo AI::negascout(Board &board, double alpha, double beta, int depth)
		       // bool isOrdering, bool isProb,
		       // int pcx, int pcy)
{
  MoveInfo info;      

  //タイムオーバーなら探索打ち切り
  if((double)((int)clock() - st.start) / (double)CLOCKS_PER_SEC
     >= st.searchTime){
    info.pos = 0;
    return info;
  }

  numSearchNode++;

  // ハッシュにヒットしてたらそこで探索おしまい
  if(bh.find(board) != end(bh)){
    BoardState bs = bh.at(board);
    if(bs.depth >= depth && bs.turn == board.getTurn()){
      info.pos = bs.pos;
      info.score = bs.score;
      // std::cout << "Hash hit!" << std::endl;
      return info;
    }
  }
  
  //リーフなら評価値を返す
  assert(depth >= 0);
  if(depth == 0 || board.isEnd()){
    info = eval(board);
    return info;
  }

  list<BitBoard> availPos;
  
  // List up all places where you can put a stone
  for(list<BitBoard>::const_iterator itr = begin(board.getCl()); 
      itr != end(board.getCl()); itr++){
    if(board.canPut(*itr)){
      availPos.push_back(*itr);      
    }
  }
  
  // 置く場所がなかったらパス
  if(availPos.size() == 0){
    assert(board.isPass());
    board.changeTurn();
    info = negascout(board, -beta, -alpha, depth - 1); 
    info.score *= -1;
    // ここでもう一度turnを変えないと、呼び出し元でのturnがおかしくなる
    board.changeTurn();
    return info;
  }

  // double margin[8] = {MARGIN3, MARGIN4, MARGIN5, MARGIN6, MARGIN7,
  // 		      MARGIN8, MARGIN9, MARGIN10};
  // //ProbCutが発動する深さ
  // // int prob_cut[8] = {3, 4, 5, 6, 7, 8, 9, 10};
  // //ProbCutのNullWindowSearchの探索深さ
  // int shallow[8] = {1, 2, 2, 3, 3, 4, 4, 5};
  // //ProbCut
  // //パスに対応するためにここに書かざるを得ない
  // if(is_prob && depth >= 3 && depth <= 10){
  //   int index = depth - 3;
  //   v = negascout(ban, turn, beta + margin[index] - DELTA, beta + margin[index], shallow[index], true, false);
  //   if(info.value >= beta + margin[index]){
  //     info.x = 0;
  //     info.y = 0;
  //     //std::cout << "prob cut b" << std::endl;
  //     return v;
  //   }
  //   v = negascout(ban, turn, alpha - margin[index], alpha - margin[index] + DELTA, shallow[index], true, false);
  //   if(info.value <= alpha - margin[index]){
  //     info.x = 0;
  //     info.y = 0;
  //     //std::cout << "prob cut a" << std::endl;
  //     return v;
  //   }
  //   //std::cout << "no prob" << std::endl;
  // }
  

  // 浅い探索によるmove ordering
  // ただし深さが小さい(具体的には3以下)ときは行わない
  if(depth >= THRESH_MOVE_ORDERING_DEPTH){  
    BitBoard revPattern;
    MoveInfo moInfo;
    map<double, BitBoard> moveScore;
    for(list<BitBoard>::iterator i = begin(availPos);
	i != end(availPos); i++){
      revPattern = board.putStone(*i);
      assert(revPattern != 0);
      // 浅い探索
      moInfo = negascout(board, -beta, -alpha, 1);
      /*
	ここではscoreに-1をかけない。
	後でmapのキーでソートを行うが、
	このとき自動的に昇順ソートになるため、
	あえて大小関係を逆にしておいた方がよいのである。
      */
      board.undo(*i, revPattern);
      moveScore[moInfo.score] = *i;
      // cout << i->first << ", " << i->second << endl;
    }
    
    availPos.clear();
    for(map<double, BitBoard>::iterator i = begin(moveScore);
	i != end(moveScore); i++){
      availPos.push_back(i->second);
      // cout << i->first << ": " << i->second.first+1 << ", "
      // 	   << i->second.second+1 << endl;
    }
  }  
  
  // first child
  // Put a stone on the first child node.
  BitBoard revPattern;

  revPattern = board.putStone(availPos.front());
  assert(revPattern != 0);
  info = negascout(board, -beta, -alpha, depth - 1);
  info.score *= -1;
  /*
    手を打つ度に盤面オブジェクトを作るのは非常にコストがかかるので、
    一手打ち、その探索が終わったら手を戻すようにしている。
  */
  board.undo(availPos.front(), revPattern);
  
  if(info.score >= beta){  // βカット
    info.pos = 0;
    return info;
  }
  if(alpha < info.score){
    alpha = info.score;
  }

  double maxScore;
  BitBoard tPos = 0;
  
  maxScore = info.score;
  tPos = availPos.front();

  
  for(list<BitBoard>::const_iterator itr = next(begin(availPos));
      itr != end(availPos); itr++){
    // // Put a stone on the child node.
    // revPattern = board.putStone(itr->first, itr->second);
    // assert(revPattern != 0);
    // info = negascout(board, -beta, -alpha, depth - 1);
    // info.score *= -1;
    // board.undo(itr->first, itr->second, revPattern);

    

    // Null Window Search
    revPattern = board.putStone(*itr);
    assert(revPattern != 0);

    info = negascout(board, -alpha - DELTA, -alpha, depth - 1);
    info.score *= -1;    
    
    if(beta <= info.score){
      board.undo(*itr, revPattern);
      info.pos = 0;
      // cout << "null beta cut" << endl;
      return info;
    }else if(alpha < info.score){
      alpha = info.score;
      info = negascout(board, -beta, -alpha, depth - 1);
      info.score *= -1;
      if(beta <= info.score){
	// cout << "real beta cut" << endl;
	board.undo(*itr, revPattern);
	info.pos = 0;
	return info;
      }else if(alpha < info.score){
	alpha = info.score;
      }
    }
    if(maxScore < info.score){      
      maxScore = info.score;
      tPos = *itr;
    }

    board.undo(*itr, revPattern);
  }

  
  info.pos = tPos;
  info.score = maxScore;
  //Hashにまだ登録されていなければ登録
  //ここに書くのは正しいのだろうか・・・
  //枝刈りされなかったときはここに来るのでよいのだが、
  //全ての子ノードの探索結果がα値を下回っていたときにもここに来るよなぁ
  //どっちにしても返すのは変数maxScoreの値なので、問題ないかな
  if(depth < 5 ||
     (double)((int)clock() - st.start) /
     (double)CLOCKS_PER_SEC >= st.searchTime){
    ; //Do nothing
  }else if(bh.find(board) == bh.end()){
    BoardState tbs(board.getTurn(), depth, info.pos, info.score);
    bh[board] = tbs;
  }else if(bh.at(board).depth < depth){
    //登録済のものよりdepthが大きければ更新
    bh.at(board).turn = board.getTurn();
    bh.at(board).depth = depth;
    bh.at(board).pos = info.pos;
    bh.at(board).score = info.score;
  }
  return info;
}

// 学習時に使用
DetailedMoveInfo AI::detailedNegascout(Board &board, double alpha, double beta, int depth, const Pattern &lnPt)
{
  DetailedMoveInfo info; 
  
  //リーフなら評価値を返す
  assert(depth >= 0);
  if(depth == 0 || board.isEnd()){
    info = detailedEval(board, lnPt);
    return info;
  }

  // これなしでも実装が可能なのでは？
  list<BitBoard> availPos;
  
  // List up all places where you can put a stone
  for(list<BitBoard>::const_iterator itr = begin(board.getCl()); 
      itr != end(board.getCl()); itr++){
    if(board.canPut(*itr)){
      availPos.push_back(*itr);      
    }
  }
  
  // 置く場所がなかったらパス
  if(availPos.size() == 0){
    assert(board.isPass());
    board.changeTurn();
    info = detailedNegascout(board, -beta, -alpha, depth - 1, lnPt); 
    info.score *= -1;
    // ここでもう一度turnを変えないと、呼び出し元でのturnがおかしくなる
    board.changeTurn();
    return info;
  }

  // 浅い探索によるmove ordering
  // ただし深さが小さい(具体的には3以下)ときは行わない
  if(depth >= THRESH_MOVE_ORDERING_DEPTH){  
    BitBoard revPattern;
    DetailedMoveInfo dmoInfo;
    map<double, BitBoard> moveScore;
    for(list<BitBoard>::iterator i = begin(availPos);
	i != end(availPos); i++){
      revPattern = board.putStone(*i);
      assert(revPattern != 0);
      // 浅い探索
      dmoInfo = detailedNegascout(board, -beta, -alpha, 1, lnPt);
      /*
	ここではscoreに-1をかけない。
	後でmapのキーでソートを行うが、
	このとき自動的に昇順ソートになるため、
	あえて大小関係を逆にしておいた方がよいのである。
      */
      board.undo(*i, revPattern);
      moveScore[dmoInfo.score] = *i;
      // cout << i->first << ", " << i->second << endl;
    }
    
    availPos.clear();
    for(map<double, BitBoard>::iterator i = begin(moveScore);
	i != end(moveScore); i++){
      availPos.push_back(i->second);
      // cout << i->first << ": " << i->second.first+1 << ", "
      // 	   << i->second.second+1 << endl;
    }
  }  
  
  // first child
  // Put a stone on the first child node.
  BitBoard revPattern;

  revPattern = board.putStone(availPos.front());
  assert(revPattern != 0);
  info = detailedNegascout(board, -beta, -alpha, depth - 1, lnPt);
  info.score *= -1;
  /*
    手を打つ度に盤面オブジェクトを作るのは非常にコストがかかるので、
    一手打ち、その探索が終わったら手を戻すようにしている。
  */
  board.undo(availPos.front(), revPattern);
  
  if(info.score >= beta){  // βカット
    info.pos = 0;
    return info;
  }
  if(alpha < info.score){
    alpha = info.score;
  }

  double maxScore;
  int tPos = 0;
  LightBoard tmpBoard;
  
  maxScore = info.score;
  tmpBoard = info.board;
  tPos = availPos.front();

  
  for(list<BitBoard>::const_iterator itr = next(begin(availPos));
      itr != end(availPos); itr++){
    // // Put a stone on the child node.
    // revPattern = board.putStone(itr->first, itr->second);
    // assert(revPattern != 0);
    // info = negascout(board, -beta, -alpha, depth - 1);
    // info.score *= -1;
    // board.undo(itr->first, itr->second, revPattern);

    

    // Null Window Search
    revPattern = board.putStone(*itr);
    assert(revPattern != 0);

    info = detailedNegascout(board, -alpha - DELTA, -alpha, depth - 1, lnPt);
    info.score *= -1;    
    
    if(beta <= info.score){
      board.undo(*itr, revPattern);
      info.pos = 0;
      // cout << "null beta cut" << endl;
      return info;
    }else if(alpha < info.score){
      alpha = info.score;
      info = detailedNegascout(board, -beta, -alpha, depth - 1, lnPt);
      info.score *= -1;
      if(beta <= info.score){
	// cout << "real beta cut" << endl;
	board.undo(*itr, revPattern);
	info.pos = 0;
	return info;
      }else if(alpha < info.score){
	alpha = info.score;
      }
    }
    if(maxScore < info.score){      
      maxScore = info.score;
      tPos = *itr;
      tmpBoard = info.board;
    }

    board.undo(*itr, revPattern);
  }

  
  info.pos = tPos;
  info.score = maxScore;
  info.board = tmpBoard;
  return info;
}


AI::AI()
{
  jouseki.readJousekiFile();
}

void AI::search(Board &board, int depth, bool is_itr)
{
  MoveInfo info, newInfo;
  numSearchNode = 0;

  st.start = (int)clock();
  // 反復深化
  if(is_itr){
    // 相手番で終わるようにしないと水平線効果でおかしな評価値になる
    for(int i = 2; i <= depth; i += 2){
      // 定石が24手までしかないからだっけ？
      if(board.getTesuu() >= 25 || !jouseki.useJouseki(board)){
	// 25手目以上ならば探索をする
	// 定石が使えなければ探索をする
	// 前の深さでの探索の結果をmove orderingに利用
	// newInfo = negascout(board, turn, -INF, INF, i,
	// 		   false, false, info.x, info.y);
	newInfo = negascout(board, -INF, INF, i);
			    // false, false, info.x, info.y);
      }else{
	// 25手目未満かつ定石が使えればreturn
	// 定石が使えるのに反復深化しても意味ないよね
	return;
      }
      if(((double)clock() - st.start) / (double)CLOCKS_PER_SEC >= st.searchTime){
	// タイムオーバーならbreak
	break;
      }else{
	// newInfoの内容をコミットする感じ
	info = newInfo;
	std::cout << "Iterative deepning : " << i << std::endl;
      }
    }
    std::cout << "search node num : " << numSearchNode << std::endl;
  }else{
    // 反復深化を行わない場合(この場合一手あたりの制限時間を設けない)
    if(board.getTesuu() >= 25 || !jouseki.useJouseki(board)){
      // 25手目以上ならば探索をする
      // 定石が使えなければ探索をする
      int tmpSt = st.searchTime;
      st.searchTime = INF;
      info = negascout(board, -INF, INF, depth);
      st.searchTime = tmpSt;
    }else{
      // 25手目未満かつ定石が使えればreturn
      return;
    }
  }
  // 結果が確定していないnewInfoを使ってはいけない
  pair<int, int> coord = Board::posToXY(info.pos);
  std::cout << "x , y, score = " << coord.first + 1 << ", " << coord.second + 1 << ", "
	    << info.score << std::endl;
  board.putStone(info.pos);
  return;
}
