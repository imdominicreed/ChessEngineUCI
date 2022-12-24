
#include "engine.hpp"

using namespace std;

int alphabetat(Board* board, int depth, int alpha, int beta,
               TranspositionTable* t, atomic<bool>* exit) {
  if (exit->load()) return -1;
  if (depth == 0) return eval(board);
  Move move_list[256];
  int num_moves = board->getMoveList(move_list);
  if (num_moves == 0) return 0;
  if (num_moves == -1) return (board->white ? 1 : -1) * (100 + checkmate);
  Entry entry = t->probe(board, depth);
  if (entry.score != INVALID && depth <= entry.depth) {
    return entry.score;
  }
  int value;
  if (entry.score != INVALID) {
    Board b = board->doMove(&entry.move);
    value = alphabetat(&b, depth - 1, alpha, beta, t, exit);
    if (board->white)
      alpha = std::max(alpha, value);
    else
      beta = std::min(beta, value);
  } else
    value = board->white ? INT32_MIN : INT32_MAX;

  Move best_move = entry.move;
  if (board->white) {
    for (int i = 0; i < num_moves; i++) {
      if (entry.move == move_list[i]) continue;
      Board b = board->doMove(&move_list[i]);
      int new_value = alphabetat(&b, depth - 1, alpha, beta, t, exit);
      if (new_value >= checkmate)
        new_value--;
      else if (new_value <= -checkmate)
        new_value++;

      if (new_value > value) {
        value = new_value;
        best_move = move_list[i];
      }
      alpha = std::max(alpha, value);
      if (value >= beta) break;
    }
    t->save(board, value, depth, best_move);
    return value;
  } else {
    for (int i = 0; i < num_moves; i++) {
      Board b = board->doMove(&move_list[i]);
      int new_value = alphabetat(&b, depth - 1, alpha, beta, t, exit);
      if (new_value >= checkmate)
        new_value--;
      else if (new_value <= -checkmate)
        new_value++;
      if (new_value < value) {
        value = new_value;
        best_move = move_list[i];
      }
      beta = std::min(beta, value);
      if (value <= alpha) break;
    }
    t->save(board, value, depth, best_move);
    return value;
  }
}

Move best_move_alphabeta_transpose(Board* board, int depth) {
  int color = board->white ? 1 : -1;
  TranspositionTable t;
  Move move_list[256];
  int best = INT32_MIN;
  Move best_move;
  int num_moves = board->getMoveList(move_list);
  for (int i = 0; i < num_moves; i++) {
    Board b = board->doMove(&move_list[i]);
    atomic<bool> exit = false;
    int move_score =
        color * alphabetat(&b, depth - 1, INT32_MIN, INT32_MAX, &t, &exit);
    if (move_score > best) {
      best_move = move_list[i];
      best = move_score;
    }
  }
  return best_move;
}
