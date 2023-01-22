
#include <semaphore>

#include "engine.hpp"

using namespace std;

#include <unistd.h>

void run_ids(Board* board, Move* best_move, atomic<bool>* exit) {
  TranspositionTable tr;
  int i = 1;
  int alpha = SMALL;
  int beta = -SMALL;
  while (!exit->load()) {
    nodes = 0;
    cerr << "Error here" << endl;

    MoveEval move_eval = best_move_alphabeta_transpose_parallel(
        board, i, &tr, exit, alpha, beta);

    int eval = move_eval.eval;
    Move move = move_eval.move;
    if (eval <= alpha || eval >= beta) {
      alpha = SMALL;
      beta = -SMALL;
      cerr << "Fell out of window researching " << i << endl;
      continue;
    }

    if (exit->load()) return;

    cerr << "Searched " << i << " Nodes searched: " << nodes
         << " Alpha: " << alpha << " Beta: " << beta << endl;
    *best_move = move;
    i++;
  }
}

Move ids_split(Board* board, int time) {
  Move best_move;
  atomic<bool> exit;
  exit.store(false);

  thread ids_thread(run_ids, board, &best_move, &exit);
  this_thread::sleep_for(chrono::milliseconds(time));
  cerr << "Exiting" << endl;
  exit.store(true);
  ids_thread.join();
  return best_move;
}
