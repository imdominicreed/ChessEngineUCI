

#include "engine.hpp"

using namespace std;

#include <unistd.h>
TranspositionTable tt;
std::atomic<bool> end_time;

void run_ids(Board* board, Move* best_move) {
  tt = TranspositionTable();
  int i = 1;
  int alpha = SMALL;
  int beta = -SMALL;
  while (!end_time.load()) {
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

    if (end_time.load()) return;

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

Move ids_split(Board* board, int time) {
  Move best_move;
  end_time.store(false);

  thread ids_thread(run_ids, board, &best_move);
  this_thread::sleep_for(chrono::milliseconds(3'000));
  cerr << "Exiting" << endl;
  end_time.store(true);
  ids_thread.join();
  return best_move;
}
