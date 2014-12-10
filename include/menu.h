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

const int MAIN_AI = 0;
const int SUB_AI = 1;

const int LEARN_VERVOSE = 1;

extern void put(Board &board, list<History> &hist, const list<string> &args);
extern void search(Board &board, AI &ai, int depth, list<History> &hist);
extern void fight(Board &board, AI &ai, AI &subAI, const list<string> &args);
extern void undo(Board &board, list<History> &hist);
extern void outputKifu(list<History> &hist);
extern void learn(const list<string> &args);
extern void load(AI &ai, AI &subAI, const list<string> &args);
