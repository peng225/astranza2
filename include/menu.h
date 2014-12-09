#pragma once

#include <list>
#include <iostream>
#include <string>
#include "board.h"
#include "ai.h"
#include "history.h"
#include "learner.h"

using std::list;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;

extern void put(Board &board, list<History> &hist, const list<string> &args);
extern void search(Board &board, AI &ai, int depth, list<History> &hist);
extern void fight(Board &board, AI &ai, const list<string> &args);
extern void undo(Board &board, list<History> &hist);
extern void outputKifu(list<History> &hist);
extern void learn(const list<string> &args);
