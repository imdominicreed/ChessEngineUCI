

#include "engine.hpp"

using namespace std;

void runalphabetat(Board board, int depth, int* ret, TranspositionTable* t,
                   atomic<bool>* exit,
                   int alpha, int beta) {
  *ret = -alphabetat(&board, depth, alpha, beta, t, exit);
  // cerr << "Releasing" << endl;

  // available->release();
}

MoveEval best_move_alphabeta_transpose_parallel(Board* board, int depth,
                                                TranspositionTable* tr,
                                                atomic<bool>* exit, int alpha,
                                                int beta) {
  // Move list
  Move move_list[256];
  Move* start = move_list;
  Move* end = board->getMoveList(move_list);

  int num_moves = end - start;

  // Threading vars
  vector<std::thread> threads;
  int futures[num_moves];

  // Create a thread for each move.
  int i = 0;
  while (start != end) {
    Board child = *board;
    child.doMove(*start);

    threads.push_back(std::thread(runalphabetat, child, depth - 1, &futures[i],
                                  tr, exit, alpha, beta));
    i++;
    start++;
  }

  Move best_move = *start;
  int bestScore = SMALL;
  for (int i = 0; i < num_moves; i++) {
    threads[i].join();
    int score = futures[i];
    if (bestScore < score) {
      bestScore = score;
      best_move = move_list[i];
    }
  }
  return {best_move, bestScore};
}
