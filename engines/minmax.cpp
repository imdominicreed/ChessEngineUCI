#include "engine.hpp"

int minmax(Board* board, int depth) {
  if (depth == 0) return eval(board);
  Move move_list[256];
  int num_moves = board->getMoveList(move_list);
  if (num_moves == 0) return 0;
  if (num_moves == -1) return (board->white ? 1 : -1) * checkmate;
  int best = board->white ? INT32_MIN : INT32_MAX;
  for (int i = 0; i < num_moves; i++) {
    Board b = board->doMove(&move_list[i]);
    int score = minmax(&b, depth - 1);
    if (board->white)
      best = std::max(best, score);
    else
      best = std::min(best, score);
  }
  return best;
}

Move best_move_minmax(Board* board, int depth) {
  int color = board->white ? 1 : -1;
  Move move_list[256];
  int best = INT32_MIN;
  Move best_move;
  int num_moves = board->getMoveList(move_list);
  for (int i = 0; i < num_moves; i++) {
    Board b = board->doMove(&move_list[i]);
    int move_score = color * minmax(&b, depth - 1);
    if (move_score > best) {
      best_move = move_list[i];
      best = move_score;
    }
  }
  return best_move;
}
