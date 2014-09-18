#include "board.h"

int main()
{
  Board b;
  b.display();
  b.putStone(5, 4, BLACK);
  b.display();
  b.putStone(3, 5, WHITE);
  b.display();
  b.putStone(0, 0, WHITE);
  b.display();
  return 0;
}
