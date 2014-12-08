#pragma once

#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include "board.h"
#include "ai.h"
#include "history.h"

using std::list;
using std::cout;
using std::cin;
using std::endl;
using std::string;

extern void put(Board &board, list<History> &hist, std::istream &ist);
extern void search(Board &board, AI &ai, int depth, list<History> &hist);
extern void fight(Board &board, AI &ai, std::istream &ist);
extern void undo(Board &board, list<History> &hist);
extern void outputKifu(list<History> &hist);
