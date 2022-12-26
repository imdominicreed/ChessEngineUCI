
#include <semaphore>

#include "engine.hpp"

using namespace std;

#include <unistd.h>

void run_ids(Board* board, Move* best_move, atomic<bool>* exit) {
  TranspositionTable tr;
  int i = 1;

  while (!exit->load()) {
    cerr << "Searching " << i << endl;
    Move new_best_move =
        best_move_alphabeta_transpose_parallel(board, i, &tr, exit);

    if (exit->load()) {
      return;
    }
    *best_move = new_best_move;
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
