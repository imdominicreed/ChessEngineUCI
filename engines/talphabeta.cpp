
#include "engine.hpp"

using namespace std;
// int Quiesce(Board* b, int alpha, int beta, TranspositionTable* tt, int depth)
// {
//   nodes++;
//   Entry entry = tt->probe(b);
//   if (entry.score != INVALID) return entry.score;

//   int stand_pat = eval(b);
//   if (depth == 3) return stand_pat;
//   alpha = max(alpha, stand_pat);
//   if (alpha >= beta) return stand_pat;

//   Move move_list[256];
//   int num_moves = b->getMoveList(move_list);

//   if (num_moves == -1) return BIG;

//   sort(move_list, num_moves, b);
//   Move best_move;
//   for (int i = 0; i < num_moves; i++) {
//     if (!move_list[i].capture) break;
//     Board new_board = b->doMove(&move_list[i]);

//     int score = -Quiesce(&new_board, -beta, -alpha, tt, depth + 1);

//     alpha = max(alpha, score);
//     if (alpha >= beta) {
//       best_move = move_list[i];
//       break;
//     }
//   }
//   tt->save(b, alpha, 256, best_move);
//   return alpha;
// }

int nodes;
int alphabetat(Board* board, int depth, int alpha, int beta,
               TranspositionTable* t, atomic<bool>* exit) {
  // Stop Searching
  if (exit->load()) return -1;
  nodes++;
  // Init Move list
  Move move_list[256];
  Move* start = move_list;
  Move* end = board->getMoveList(move_list);
   // Root Node Check
  if (end == nullptr) return BIG;
  if (depth == 0) return eval(board);  // return Quiesce(board, alpha, beta, t, 0);

  // Table check
  // Entry entry = t->probe(board);
  // Move best_move;
  int best_score = SMALL;
  // if (entry.score != INVALID && entry.depth <= depth) return entry.score;

  // Null move
  // if (depth >= 3 && !board->inCheck()) {
  //   board->turn = (Color)(board->turn ^ 1);
  //   int score = -alphabetat(board, depth - 3, -beta, -beta + 1, t, exit);
  //   board->turn = (Color)(board->turn ^ 1);
  //   if (score >= beta) return beta;
  // }

  // if (entry.score != INVALID) {
  //   best_move = entry.move;

  //   UndoMove undo = board->doMove(best_move);
  //   best_score = -alphabetat(board, depth - 1, -beta, -alpha, t, exit);
  //   board->undoMove(undo);

  //   alpha = max(alpha, best_score);
  //   if (alpha >= beta) return best_score;
  // }

  // move_sort(start, end, board);

  // Search child Nodes
  while (start != end) {
    // if (*start == entry.move) continue;
    // Do move
    UndoMove undo = board->doMove(*start);
    int score = -alphabetat(board, depth - 1, -beta, -alpha, t, exit);
    board->undoMove(undo);

    best_score = max(score, best_score);

    // Alpha Beta Pruning
    alpha = max(alpha, score);

    if (alpha >= beta) {
      break;
    }
    start++;
  }
  // t->save(board, best_score, depth, best_move);
  return best_score;
}
