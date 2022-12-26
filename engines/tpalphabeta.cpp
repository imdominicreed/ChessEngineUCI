
#include <semaphore>

#include "engine.hpp"

using namespace std;

void runalphabetat(Board board, int depth, int* ret, TranspositionTable* t,
                   atomic<bool>* exit, counting_semaphore<3>* available) {
  *ret = -alphabetat(&board, depth, SMALL, -SMALL, t, exit);
  // cerr << "Releasing" << endl;

  // available->release();
}

Move best_move_alphabeta_transpose_parallel(Board* board, int depth,
                                            TranspositionTable* tr,
                                            atomic<bool>* exit) {
  // Move list
  Move move_list[256];
  int num_moves = board->getMoveList(move_list);

  // Threading vars
  vector<std::thread> threads;
  int futures[num_moves];
  counting_semaphore<3> available(3);

  // Create a thread for each move.
  for (int i = 0; i < num_moves; i++) {
    Board b = board->doMove(&move_list[i]);
    // cerr << "Trying to acquire!" << endl;
    // available.acquire();
    // cerr << "I acquired!" << endl;

    threads.push_back(std::thread(runalphabetat, b, depth - 1, &futures[i], tr,
                                  exit, &available));
  }

  Move best_move;
  int bestScore = SMALL;
  for (int i = 0; i < num_moves; i++) {
    threads[i].join();
    int score = futures[i];
    if (bestScore < score) {
      bestScore = score;
      best_move = move_list[i];
    }
  }
  return best_move;
}
