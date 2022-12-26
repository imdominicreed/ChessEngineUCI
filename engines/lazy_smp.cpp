
#include <semaphore>

#include "engine.hpp"

using namespace std;

#include <unistd.h>

void run(Board* board, Move* best_move, int* eval, TranspositionTable* t,
         atomic<bool>* exit, int depth) {
  *eval = INT32_MIN;
  int color = board->white ? 1 : -1;

  while (!exit->load()) {
    cerr << "Depth: " << depth << endl;
    Move move_list[256];
    int best = INT32_MIN;
    Move it_best_move;

    int num_moves = board->getMoveList(move_list);
    for (int i = 0; i < num_moves; i++) {
      Board b = board->doMove(&move_list[i]);
      int move_score =
          color * alphabetat(&b, depth - 1, INT32_MIN, INT32_MAX, t, exit);
      if (move_score > best) {
        it_best_move = move_list[i];
        best = move_score;
      }
    }

    if (exit->load()) {
      return;
    }

    *eval = INT32_MIN;
    cerr << "Eval: " << best << endl;

    if (*eval < best) {
      *eval = best;
      *best_move = it_best_move;
    }

    depth++;
  }
}

int NUM_THREADS = 6;
Move lazy_smp(Board* board, int time) {
  atomic<bool> exit;
  exit.store(false);
  pair<Move, int> moves[NUM_THREADS];
  vector<thread> threads;
  TranspositionTable t = TranspositionTable();
  int depth = 0;
  for (int i = 0; i < NUM_THREADS; i++) {
    if (i % 1 == 0) depth++;

    threads.push_back(thread(run, board, &moves[i].first, &moves[i].second, &t,
                             &exit, depth));
  }

  this_thread::sleep_for(chrono::milliseconds(time));
  exit.store(true);
  Move best_move;
  int eval = INT32_MIN;
  for (int i = 0; i < NUM_THREADS; i++) {
    threads[i].join();

    if (eval < moves[i].second) {
      eval = moves[i].second;
      best_move = moves[i].first;
    }
  }

  return best_move;
}
