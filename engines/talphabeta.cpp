
#include "engine.hpp"

using namespace std;
int Quiesce(Board* b, int alpha, int beta, int depth) {
  nodes++;
  Move move_list[256];
  Move* start = move_list;
  Move* end = b->getMoveList(move_list);
  if (b->isStaleMate() || start == end) return 0;

  int stand_pat = eval(b);
  if (stand_pat >= beta) return beta;
  alpha = max(alpha, stand_pat);

  if (end == nullptr) return BIG;

  move_sort(start, end, b);
  while (start != end) {
    if (!is_capture(*start)) break;
    UndoMove undo = b->doMove(*start);

    int score = -Quiesce(b, -beta, -alpha, depth + 1);
    b->undoMove(undo);

    if (score >= beta) return beta;
    if (score > alpha) alpha = score;
    start++;
  }

  return alpha;
}

int nodes;
int tbl_hits;
int alphabetat(Board* board, int depth, int alpha, int beta) {
  // Stop Searching
  if ((nodes & 1023) == 1023) {
    check_time();
  }
  if (out_of_time()) return -1;
  nodes++;

  // Init Move list
  Move move_list[256];
  Move* start = move_list;
  Move* end = board->getMoveList(move_list);

  if (board->isStaleMate() || start == end) return 0;

  // Root Node Check
  if (end == nullptr) {
    return BIG;
  }
  if (depth == 0) return eval(board);

  // Table check
  Entry entry = tt.probe(board);
  Move best_move = 0;
  int best_score = SMALL;
  if (entry.depth() != INVALID_DEPTH && entry.depth() >= depth) {
    tbl_hits++;
    return entry.score();
  }

  move_sort(start, end, board);

  // Search child Nodes
  bool checkmate = true;
  while (start != end) {
    // Do move
    UndoMove undo = board->doMove(*start);
    int score = -alphabetat(board, depth - 1, -beta, -alpha);
    board->undoMove(undo);
    checkmate &= score == -BIG;

    if (score > CHECKMATE)
      score--;
    else if (score < -CHECKMATE)
      score++;

    if (best_score < score) {
      best_score = score;
      best_move = *start;
    }

    // Alpha Beta Pruning
    alpha = max(alpha, score);

    if (!checkmate && alpha >= beta) {
      break;
    }
    start++;
  }

  if (checkmate && !board->currentCheck()) {
    best_score = 0;
  }
  tt.save(board, best_score, depth, best_move, NodeType::PV);
  return best_score;
}
