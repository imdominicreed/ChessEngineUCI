
#include "engine.hpp"

using namespace std;
// int Quiesce(Board* b, int alpha, int beta, int depth) {
//   nodes++;
//   Entry entry = tt.probe(b);
//   // if (entry.depth() != INVALID_DEPTH) return entry.score();

//   int stand_pat = eval(b);
//   if (depth == 3) return stand_pat;
//   alpha = max(alpha, stand_pat);
//   if (alpha >= beta) return stand_pat;

//   Move move_list[256];
//   Move* start = move_list;
//   Move* end = b->getMoveList(move_list);

//   if (end == nullptr) return BIG;

//   move_sort(start, end, b);
//   Move best_move;
//   while (start != end) {
//     if (is_capture(*start)) break;
//     UndoMove undo = b->doMove(*start);

//     int score = -Quiesce(b, -beta, -alpha, depth + 1);
//     b->undoMove(undo);

//     alpha = max(alpha, score);
//     if (alpha >= beta) {
//       best_move = *start;
//       break;
//     }
//     start++;
//   }
//   tt.save(b, alpha, 0, best_move, NodeType::All);
//   return alpha;
// }

int nodes;
int tbl_hits;
int alphabetat(Board* board, int depth, int alpha, int beta) {
  // Stop Searching
  if ((nodes & 1023) == 1023) {
    check_time();
  }
  if (out_of_time()) return -1;
  nodes++;
  if (board->isStaleMate()) return 0;
  // Init Move list
  Move move_list[256];
  Move* start = move_list;
  Move* end = board->getMoveList(move_list);
  // Root Node Check
  if (end == nullptr) {
    return BIG;
  }
  if (depth == 0) return eval(board);

  // Table check
  Entry entry = tt.probe(board);
  Move best_move = *start;
  int best_score = SMALL;
  if (entry.depth() != INVALID_DEPTH && entry.depth() >= depth) {
    tbl_hits++;
    // return entry.score();
  }

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

  move_sort(start, end, board);

  // Search child Nodes
  while (start != end) {
    // if (*start == entry.move) {
    //   start++;
    //   continue;
    // }
    // Do move
    UndoMove undo = board->doMove(*start);
    int score = -alphabetat(board, depth - 1, -beta, -alpha);
    board->undoMove(undo);
    if (score > checkmate)
      score--;
    else if (score < -checkmate)
      score++;
    if (best_score < score) {
      best_score = score;
      best_move = *start;
    }

    // Alpha Beta Pruning
    alpha = max(alpha, score);

    if (alpha >= beta) {
      break;
    }
    start++;
  }
  tt.save(board, best_score, depth, best_move, NodeType::PV);
  return best_score;
}
