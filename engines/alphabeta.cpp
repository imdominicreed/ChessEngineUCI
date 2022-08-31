#include "engine.hpp"

int alphabeta(Board* board, int depth, int alpha, int beta) {
  if (depth == 0) return eval(board);
  Move move_list[256];
  int num_moves = get_move_list(board, move_list);
  if (num_moves == 0) return 0;
  if (num_moves == -1) return (board->white ? 1 : -1) * checkmate;
  if (board->white) {
    int value = INT32_MIN;
    for (int i = 0; i < num_moves; i++) {
      Board b = do_move(&move_list[i], *board);
      value = std::max(value, alphabeta(&b, depth - 1, alpha, beta));
      alpha = std::max(alpha, value);
      if (value >= beta) break;
    }
    return value;
  } else {
    int value = INT32_MAX;
    for (int i = 0; i < num_moves; i++) {
      Board b = do_move(&move_list[i], *board);
      value = std::min(value, alphabeta(&b, depth - 1, alpha, beta));
      beta = std::min(beta, value);
      if (value <= alpha) break;
    }
    return value;
  }
}

Move best_move_alphabeta(Board* board, int depth) {
  int color = board->white ? 1 : -1;
  Move move_list[256];
  int best = INT32_MIN;
  Move best_move;
  int num_moves = get_move_list(board, move_list);
  for (int i = 0; i < num_moves; i++) {
    Board b = do_move(&move_list[i], *board);
    int move_score = color * alphabeta(&b, depth - 1, INT32_MIN, INT32_MAX);
    if (move_score > best) {
      best_move = move_list[i];
      best = move_score;
    }
  }
  return best_move;
}
