#include "ChessBoardAPI/src/move.hpp"
#include "ChessBoardAPI/src/board.hpp"
const int checkmate = 10000000;
int minmax(Board* board, int depth);
typedef Move (*ftype)(Board*, int);
Move best_move_parallelminmax(Board* board, int depth);
Move best_move_minmax(Board* board, int depth);


