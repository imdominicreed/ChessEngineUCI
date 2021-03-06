#include "ChessBoardAPI/src/move.hpp"
#include "ChessBoardAPI/src/board.hpp"
#include "tt.hpp"

const int checkmate = 10000000;
int alphabetat(Board* board, int depth, int alpha, int beta, TranspositionTable* t);
int alphabeta(Board* board, int depth, int alpha, int beta);
int minmax(Board* board, int depth);
typedef Move (*ftype)(Board*, int);
Move best_move_parallelminmax(Board* board, int depth);
Move best_move_minmax(Board* board, int depth);
Move best_move_alphabeta(Board* board, int depth);
Move best_move_parallelalphabeta(Board* board, int depth);
Move best_move_alphabeta_transpose(Board* board, int depth);
Move best_move_alphabeta_transpose_parallel(Board* board, int depth);
