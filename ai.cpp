#include "ai.h"
#include <map>
// #include <algorithm>

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
    info.board = board;
    return info;
  }
  // ここはなんで+=なんだ？
  info.score += pt.evalFeature(board);
  info.board = board;
  return info;
}


// NegaScout
// 主にプレイ時に使用
/*
  パスの仕様を変更し、
  turnやtesuuをBoardクラス内で自動的に変更するようにしたため、
  探索時にパスをどのように検出するかが問題となる。
  どうしよう。
*/
MoveInfo AI::negascout(Board &board, int alpha, int beta, int depth)
		       // bool isOrdering, bool isProb,
		       // int pcx, int pcy)
{
  MoveInfo info;      

  //タイムオーバーなら探索打ち切り
  if((double)((int)clock() - st.start) / (double)CLOCKS_PER_SEC
     >= st.searchTime){
    info.x = 0;
    info.y = 0;
    return info;
  }

  numSearchNode++;

  // ハッシュにヒットしてたらそこで探索おしまい
  if(bh.find(board) != end(bh)){
    BoardState bs = bh.at(board);
    if(bs.depth >= depth && bs.turn == board.getTurn()){
      info.x = bs.x;
      info.y = bs.y;
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

  list<pair<int, int> > availPos;
  
  // List up all places where you can put a stone
  for(list<std::pair<int, int> >::const_iterator itr = begin(board.getCl()); 
      itr != end(board.getCl()); itr++){
    if(board.canPut(itr->first, itr->second)){
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
  // ただし深さが小さい(具体的には2以下)ときは行わない
  // ただし反復深化によるmove orderingが優先
  if(depth >= THRESH_MOVE_ORDERING_DEPTH){  
    BitBoard revPattern;
    MoveInfo moInfo;
    map<int, pair<int, int> > moveScore;
    for(list<pair<int, int> >::iterator i = begin(availPos);
	i != end(availPos); i++){
      revPattern = board.putStone(i->first, i->second);
      assert(revPattern != 0);
      // 浅い探索
      moInfo = negascout(board, -beta, -alpha, 1);
      /*
	ここではscoreに-1をかけない。
	後でmapのキーでソートを行うが、
	このとき自動的に昇順ソートになるため、
	あえて大小関係を逆にしておいた方がよいのである。
      */
      board.undo(i->first, i->second, revPattern);
      moveScore[moInfo.score] = *i;
      // cout << i->first << ", " << i->second << endl;
    }
    
    availPos.clear();
    for(map<int, pair<int, int> >::iterator i = begin(moveScore);
	i != end(moveScore); i++){
      availPos.push_back(i->second);
      // cout << i->first << ": " << i->second.first+1 << ", "
      // 	   << i->second.second+1 << endl;
    }
  }  
  
  // first child
  // Put a stone on the first child node.
  BitBoard revPattern;

  revPattern = board.putStone(availPos.front().first,
			      availPos.front().second);
  assert(revPattern != 0);
  info = negascout(board, -beta, -alpha, depth - 1);
  info.score *= -1;
  /*
    手を打つ度に盤面オブジェクトを作るのは非常にコストがかかるので、
    一手打ち、その探索が終わったら手を戻すようにしている。
  */
  board.undo(availPos.front().first, availPos.front().second, revPattern);
  
  if(info.score >= beta){  // βカット
    info.x = 0;
    info.y = 0;
    return info;
  }
  if(alpha < info.score){
    alpha = info.score;
  }

  // MoveInfoが盤面情報を持つ必要ってあるのか？
  // 学習に必要とか？
  double maxScore;
  int tx = 0, ty = 0;
  Board tmpBoard;
  
  maxScore = info.score;
  tmpBoard = info.board;
  tx = availPos.front().first;
  ty = availPos.front().second;

  
  for(list<pair<int, int> >::const_iterator itr = next(begin(availPos));
      itr != end(availPos); itr++){
    // // Put a stone on the child node.
    // revPattern = board.putStone(itr->first, itr->second);
    // assert(revPattern != 0);
    // info = negascout(board, -beta, -alpha, depth - 1);
    // info.score *= -1;
    // board.undo(itr->first, itr->second, revPattern);

    

    // Null Window Search
    revPattern = board.putStone(itr->first, itr->second);
    assert(revPattern != 0);

    info = negascout(board, -alpha - 1, -alpha, depth - 1);
    info.score *= -1;    
    
    if(beta <= info.score){
      board.undo(itr->first, itr->second, revPattern);
      info.x = 0;
      info.y = 0;
      return info;
    }else if(alpha < info.score){
      alpha = info.score;
      info = negascout(board, -beta, -alpha, depth - 1);
      info.score *= -1;
      if(beta <= info.score){
	board.undo(itr->first, itr->second, revPattern);
	info.x = 0;
	info.y = 0;
	return info;
      }else if(alpha < info.score){
	alpha = info.score;
      }
    }
    if(maxScore < info.score){      
      maxScore = info.score;
      tx = itr->first;
      ty = itr->second;
      tmpBoard = info.board;
    }

    board.undo(itr->first, itr->second, revPattern);
  }

  
  info.x = tx;
  info.y = ty;
  info.score = maxScore;
  info.board = tmpBoard;
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
    BoardState tbs(board.getTurn(), depth, info.x, info.y, info.score);
    bh[board] = tbs;
  }else if(bh.at(board).depth < depth){
    //登録済のものよりdepthが大きければ更新
    bh.at(board).turn = board.getTurn();
    bh.at(board).depth = depth;
    bh.at(board).x = info.x;
    bh.at(board).y = info.y;
    bh.at(board).score = info.score;
  }
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
      // 恐らくこれが使いたいときはProbCutはしたくない
      // info = negascout(board, turn, -INF, INF, depth, true, false);
      info = negascout(board, -INF, INF, depth);
		       // , true, false);
      st.searchTime = tmpSt;
    }else{
      // 25手目未満かつ定石が使えればreturn
      return;
    }
  }
  // 結果が確定していないnewInfoを使ってはいけない
  std::cout << "x , y, score = " << info.x << ", " << info.y << ", "
	    << info.score << std::endl;
  board.putStone(info.x, info.y);
  return;
}

// //Search2
// Value Search2::eval(const Board &board, int turn)
// {
//   Value v;
//   v.value = (turn == BLACK ? pt.evalFeature(board) : pt.evalFeature(board) * (-1));
//   v.board = board;
//   return v;
// }


// //NegaScout(都合上、関数の名前はNegaMaxになっているorz)
// //主に学習時に使用
// Value Search2::negascout(const Board &board, int turn, double alpha, double beta, 
// 			  int depth, bool is_ordering, bool is_prob,
// 			  int pcx, int pcy)
// {
//   Value v;
//   std::vector<int> x;
//   std::vector<int> y;
//   int tx = 0, ty = 0;
//   spBan tboard = spBan(new Ban());

//   //リーフなら評価値を返す
//   if(depth == 0 || board->is_end()){
//     v = eval(board, turn);
//     return v;
//   }
  
//   //List up all places where you can put a stone
//   for(list<std::pair<int, int> >::iterator itr = board->get_cl().begin(); 
//       itr != board->get_cl().end(); itr++){
//     if(board->can_put(itr->first, itr->second, turn)){
//       x.push_back(itr->first);
//       y.push_back(itr->second);
//     }
//   }
//   // for(int i = 1; i < BAN_SIZE - 1; i++){
//   //   for(int j = 1; j < BAN_SIZE - 1; j++){
//   //     if(board->can_put(i, j, turn)){
//   // 	x.push_back(i);
//   // 	y.push_back(j);
//   //     }
//   //   }
//   // }
  
//   //If there is no place to put a stone, then pass.
//   if(x.size() == 0){  
//     v = negascout(board, -turn, -beta, -alpha, depth - 1); 
//     v.value *= -1;
//     return v;
//   }

//   /*
//     double margin[7] = {MARGIN4, MARGIN5, MARGIN6, MARGIN7,
//     MARGIN8, MARGIN9, MARGIN10};
//     //ProbCutが発動する深さ
//     int prob_cut[7] = {4, 5, 6, 7, 8, 9, 10};
//     //ProbCutのNullWindowSearchの探索深さ
//     int shallow[7] = {2, 3, 3, 4, 4, 5, 5};
//     //ProbCut
//     //パスに対応するためにここに書かざるを得ない
//     if(false && is_prob){
//     for(int i = 0; i < 7; i++){
//     if(depth == prob_cut[i]){
//     v = negascout(board, turn, beta + margin[i] - DELTA, beta + margin[i], shallow[i], true, false);
//     if(v.value >= beta + margin[i]){
//     v.x = 0;
//     v.y = 0;
//     //std::cout << "prob cut b" << std::endl;
//     return v;
//     }
//     v = negascout(board, turn, alpha - margin[i], alpha - margin[i] + DELTA, shallow[i], true, false);
//     if(v.value <= alpha - margin[i]){
//     v.x = 0;
//     v.y = 0;
//     //std::cout << "prob cut a" << std::endl;
//     return v;
//     }
//     //std::cout << "no prob" << std::endl;
//     }
//     }
//     }*/

//   //浅い探索によるmove ordering
//   //ただし深さが小さい(具体的には3以下)ときは行わない
//   if(is_ordering && depth >= 4){
//     Value tv;
//     tv = negascout(board, turn, alpha, beta, 2);  //ここは tv.value *= -1 してはいけない！
//     //浅い探索の結果、長兄ノードがPVノードならば何もしない
//     if(tv.x != x.at(0) || tv.y != y.at(0)){
//       //次兄ノード以降を調べる
//       for(int i = 1; i < (signed)x.size(); i++){
// 	//PVノードと長兄ノードを入れ替える
// 	if(tv.x == x.at(i) && tv.y == y.at(i)){
// 	  iter_swap(x.begin(), x.begin() + i);
// 	  iter_swap(y.begin(), y.begin() + i);
// 	  break;
// 	}
//       } 
//     }
//   }

//   spBan c_board = spBan(new Ban());
  
//   //first child
//   //Put a stone on the first child node.
//   c_board->copy_board(board);
//   c_board->put_stone(x.at(0), y.at(0), turn);
//   double max;
//   v = negascout(c_board, -turn, -beta, -alpha, depth - 1);
//   v.value *= -1;
//   max = v.value;
//   tboard->copy_board(v.board);
//   tx = x.at(0);
//   ty = y.at(0);
//   if(v.value >= beta){  //βカット
//     v.x = 0;
//     v.y = 0;
//     return v;
//   }
//   if(alpha < v.value){
//     alpha = v.value;
//   }
  
//   for(int i = 0; i < (signed)x.size(); i++){
//     //Put a stone on the child node.
//     c_board->copy_board(board);
//     c_board->put_stone(x.at(i), y.at(i), turn);
//     //scout
//     v = negascout(c_board, -turn, -(alpha + DELTA), -alpha, depth - 1, true, false);
//     v.value *= -1;
//     if(v.value >= beta){  //βカット
//       v.x = 0;
//       v.y = 0;
//       return v;
//     }
//     if(alpha < v.value){
//       alpha = v.value;
//       v = negascout(c_board, -turn, -beta, -alpha, depth - 1);
//       v.value *= -1;
//       //かなり怪しい
//       //assert(alpha <= v.value);
//       //assert入れると落ちるのでif文で苦し紛れの処理
//       if(alpha <= v.value){
// 	alpha = v.value;
//       }
//       if(alpha >= beta){  //βカット
// 	v.x = 0;
// 	v.y = 0;
// 	return v;
//       }
//     }
//     if(max < v.value){
//       max = v.value;
//       tx = x.at(i);
//       ty = y.at(i);
//       tboard->copy_board(v.board);
//     }
//     /*
//       v = negascout(c_board, -turn, -beta, -alpha, depth - 1);
//       v.value *= -1;
//       if(v.value >= beta){  //βカット
//       exit(0);
//       v.x = 0;
//       v.y = 0;
//       return v;
//       }
//       if(alpha < v.value){
//       alpha = v.value;
//       tx = v.x;
//       ty = v.y;
//       //max = alpha;
//       tboard->copy_board(v.board);
//       }
//     */
//   }
//   v.x = tx;
//   v.y = ty;
//   v.value = max;
//   v.board->copy_board(tboard);
//   return v;
// }
