
#include "engine.hpp"

using namespace std;
int Quiesce(Board* b, int alpha, int beta) {
  int stand_pat = eval(b);
  alpha = max(alpha, stand_pat);
  if (alpha >= beta) return stand_pat;

  Move move_list[256];
  int num_moves = b->getMoveList(move_list);

  if (num_moves == -1) return BIG;

  for (int i = 0; i < num_moves; i++) {
    if (!move_list[i].capture) continue;
    Board new_board = b->doMove(&move_list[i]);

    int score = -Quiesce(&new_board, -beta, -alpha);

    alpha = max(alpha, score);
    if (alpha >= beta) break;
  }
  return alpha;
}

int alphabetat(Board* board, int depth, int alpha, int beta,
               TranspositionTable* t, atomic<bool>* exit) {
  // Stop Searching
  if (exit->load()) return -1;

  // Init Move list
  Move move_list[256];
  int num_moves = board->getMoveList(move_list);

  // Root Node Check
  if (num_moves == -1) return BIG;
  if (depth == 0) return eval(board);

  // Table check
  Entry entry = t->probe(board);
  Move best_move;
  int best_score = SMALL;
  if (entry.score != INVALID && entry.depth <= depth)
    return entry.score;

  else if (entry.score != INVALID) {
    best_move = entry.move;
    Board child = board->doMove(&best_move);
    best_score = -alphabetat(&child, depth - 1, -beta, -alpha, t, exit);
    alpha = max(alpha, best_score);
    if (alpha >= beta) return best_score;
  }

  // if (depth >= 3) {
  //   Board child = *board;
  //   child.white = !child.white;
  //   int score = -alphabetat(&child, depth - 3, -beta, -beta + 1, t, exit);
  //   if (score >= alpha) return beta;
  // }

  // Search child Nodes
  for (int i = 0; i < num_moves; i++) {
    if (move_list[i] == entry.move) continue;
    // Do move
    Board child = board->doMove(&move_list[i]);
    int score = -alphabetat(&child, depth - 1, -beta, -alpha, t, exit);
    best_score = max(score, best_score);

    // Alpha Beta Pruning
    alpha = max(alpha, score);

    if (alpha >= beta) {
      break;
    }
  }
  t->save(board, best_score, depth, best_move);
  return best_score;
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
        color * alphabetat(&b, depth - 1, SMALL, -SMALL, &t, &exit);
    if (move_score > best) {
      best_move = move_list[i];
      best = move_score;
    }
  }
  return best_move;
}
