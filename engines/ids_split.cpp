

#include "engine.hpp"

using namespace std;

#include <unistd.h>
TranspositionTable tt;

void run_ids(Board* board, Move* best_move) {
  tt = TranspositionTable();
  int i = 1;
  int alpha = SMALL;
  int beta = -SMALL;
  cerr << out_of_time() << endl;

  while (!out_of_time()) {
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

    cerr << "Searched " << i << " Nodes searched: " << nodes
         << " Eval: " << eval << " Table hits: " << tbl_hits
         << " pv: " << to_string(move) << " Mini eval: ";

    Entry e;
    vector<UndoMove> stack;
    UndoMove undo = board->doMove(move);
    while ((e = tt.probe(board)).depth() != INVALID_DEPTH) {
      cerr << to_string(e.move()) << " " << e.depth() << " ";
      stack.push_back(board->doMove(e.move()));
    }
    cerr << e.depth();
    cerr << endl;

    while (stack.size()) {
      board->undoMove(stack.back());
      stack.pop_back();
    }
    board->undoMove(undo);
    *best_move = move;
    i++;
  }
}

Move ids_split(Board* board) {
  Move best_move;

  run_ids(board, &best_move);
  cout << "bestmove " << to_string(best_move) << endl;
  return best_move;
}
