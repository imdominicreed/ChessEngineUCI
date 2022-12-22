
#include "engine.hpp"

using namespace std;

void runalphabetat(Board board, int depth, int* ret, TranspositionTable* t) {
  *ret = alphabetat(&board, depth, INT32_MIN, INT32_MAX, t);
}

Move best_move_alphabeta_transpose_parallel(Board* board, int depth) {
  int color = board->white ? 1 : -1;
  Move move_list[256];
  TranspositionTable tr = TranspositionTable();
  int best = INT32_MIN;
  Move best_move;
  int num_moves = board->getMoveList(move_list);
  vector<std::thread> threads;
  int futures[num_moves];
  for (int i = 0; i < num_moves; i++) {
    Board b = board->doMove(&move_list[i]);
    threads.push_back(
        std::thread(runalphabetat, b, depth - 1, &futures[i], &tr));
  }

  cerr << "\nNew Move Sequence\n";
  for (int i = 0; i < num_moves; i++) {
    threads[i].join();
    cerr << "before access\n";
    cerr << move_list[i].toString();
    cerr << "\n after acess\n";
    int data = futures[i];
    int eval = data * color;
    cerr << "Evaluation: " << eval << " Move: " << move_list[i].toString()
         << " Line: ";
    cerr << endl;
    if (eval > best) {
      best = eval;
      best_move = move_list[i];
    }
  }
  return best_move;
}
