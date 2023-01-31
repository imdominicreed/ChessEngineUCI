#pragma once

#include <stdint.h>

#include <algorithm>  // std::max
#include <atomic>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <thread>
#include <utility>
#include <vector>

#include "../ChessBoardAPI/board/board.hpp"
#include "../ChessBoardAPI/move/move.hpp"
#include "../ChessBoardAPI/move/move_gen.hpp"
#include "../eval/eval.hpp"
#include "../eval/move_picker.hpp"
#include "tt.hpp"

const int checkmate = 1000000;
const int SMALL = -100000000;
const int BIG = 10000000;
extern int nodes;
extern int tbl_hits;
extern TranspositionTable tt;

struct MoveEval {
  Move move;
  int eval;
};

int alphabetat(Board* board, int depth, int alpha, int beta);
int alphabeta(Board* board, int depth, int alpha, int beta);
int minmax(Board* board, int depth);
MoveEval best_move_alphabeta_transpose_parallel(Board* board, int depth,
                                                int alpha, int beta);
MoveEval negamax(Board* b, int depth);
Move best_move_parallelminmax(Board* board, int depth);
Move best_move_minmax(Board* board, int depth);
Move best_move_alphabeta(Board* board, int depth);
Move best_move_parallelalphabeta(Board* board, int depth);
Move best_move_alphabeta_transpose(Board* board, int depth);

Move ids_split(Board* board);
Move lazy_smp(Board* board, int time);

void set_remaining_time(int time, int num_moves);
bool out_of_time();
void check_time();
void set_move_time(int time);
