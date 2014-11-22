#include "history.h"

History::History(const Board &board)
{
  this->board = board;
}

void History::recover(Board& board) const
{
  board = this->board;
}
