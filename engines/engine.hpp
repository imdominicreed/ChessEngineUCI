#include <stdint.h>
#pragma once
#include <algorithm>  // std::max
#include <atomic>
#include <fstream>
#include <future>
#include <iostream>
#include <semaphore>
#include <sstream>
#include <thread>
#include <utility>
#include <vector>

#include "../ChessBoardAPI/board/board.hpp"
#include "../ChessBoardAPI/move/move.hpp"
#include "../ChessBoardAPI/move/move_gen.hpp"
#include "../eval/eval.hpp"
#include "tt.hpp"

const int checkmate = 10000000;

int alphabetat(Board* board, int depth, int alpha, int beta,
               TranspositionTable* t, std::atomic<bool>* exit);
int alphabeta(Board* board, int depth, int alpha, int beta);
int minmax(Board* board, int depth);
Move best_move_alphabeta_transpose_parallel(Board* board, int depth,
                                            TranspositionTable* tr,
                                            std::atomic<bool>* exit);
Move best_move_parallelminmax(Board* board, int depth);
Move best_move_minmax(Board* board, int depth);
Move best_move_alphabeta(Board* board, int depth);
Move best_move_parallelalphabeta(Board* board, int depth);
Move best_move_alphabeta_transpose(Board* board, int depth);

typedef Move (*ftype)(Board*, int);
Move ids_split(Board* board, int time);
Move lazy_smp(Board* board, int time);
