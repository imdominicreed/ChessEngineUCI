
#include <semaphore>

#include "engine.hpp"

using namespace std;
counting_semaphore<8> available(8);

void runalphabetat(Board board, int depth, int* ret, TranspositionTable* t) {
  *ret = alphabetat(&board, depth, INT32_MIN, INT32_MAX, t);
  available.release();
}

Move best_move_alphabeta_transpose_parallel(Board* board, int depth,
                                            TranspositionTable* tr) {
  int color = board->white ? 1 : -1;
  Move move_list[256];
  int best = INT32_MIN;
  Move best_move;
  int num_moves = board->getMoveList(move_list);
  vector<std::thread> threads;
  int futures[num_moves];
  for (int i = 0; i < num_moves; i++) {
    Board b = board->doMove(&move_list[i]);
    available.acquire();
    threads.push_back(
        std::thread(runalphabetat, b, depth - 1, &futures[i], tr));
  }

  for (int i = 0; i < num_moves; i++) {
    threads[i].join();
    int data = futures[i];
    int eval = data * color;
    if (eval > best) {
      best = eval;
      best_move = move_list[i];
    }
  }
  cerr << "Eval: " << best << endl;
  return best_move;
}
