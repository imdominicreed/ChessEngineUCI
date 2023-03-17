#include "../ChessBoardAPI/board/board.hpp"
#include "../ChessBoardAPI/util/bitutil.hpp"
#include "./nnue/nnue.h"

int eval(Board* board);

int eval_nnue(Board* b);

void init_nnue(char* file_path);