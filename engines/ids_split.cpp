

#include "engine.hpp"

using namespace std;

#include <unistd.h>
TranspositionTable tt;

void run_ids(Board* board, Move* best_move) {
  tt.clear();
  int i = 1;
  int alpha = SMALL;
  int beta = -SMALL;
  overwrite = 0;

  cerr << "cm: " << (BIG - 1) << endl;
  while (!out_of_time() && i < 256) {
    cerr << i << endl;
    nodes = 0;

    tbl_hits = 0;
    MoveEval move_eval =
        best_move_alphabeta_transpose_parallel(board, i, alpha, beta);
    // MoveEval mini_eval = negamax(board, i);

    int eval = move_eval.eval;
    Move move = move_eval.move;
    if (eval <= alpha || eval >= beta) {
      alpha = SMALL;
      beta = -SMALL;
      cerr << "Fell out of window researching " << i << endl;
      continue;
    }

    if (out_of_time()) return;

    cout << "info depth " << i << " time " << get_time_searched() << " nodes "
         << nodes << " pv " << to_string(move);
    Entry e;
    vector<UndoMove> stack;
    UndoMove undo = board->doMove(move);
    int pv_depth = i - 1;
    while ((e = tt.probe(board)).depth() != INVALID_DEPTH && pv_depth >= 0) {
      cout << " " << to_string(e.move());
      stack.push_back(board->doMove(e.move()));
      pv_depth--;
    }

    while (stack.size()) {
      board->undoMove(stack.back());
      stack.pop_back();
    }
    board->undoMove(undo);
    *best_move = move;

    cout << " score ";
    if (abs(eval) >= CHECKMATE)
      cout << "mate " << (eval >= 0 ? 1 : -1) * (BIG - abs(eval));
    else
      cout << "cp " << eval;

    cout << " hashfull " << (tt.hash_full * 1000LL) / (tt.SIZE * 8LL) << endl;
    i++;
  }
}

Move ids_split(Board* board) {
  Move best_move;

  run_ids(board, &best_move);
  cout << "bestmove " << to_string(best_move) << endl;
  return best_move;
}
