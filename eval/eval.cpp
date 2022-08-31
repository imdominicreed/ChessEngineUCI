#include <iostream>

#include "../ChessBoardAPI/src/bitutil.hpp"
#include "../ChessBoardAPI/src/board.hpp"

int count(bitboard num) { return __builtin_popcountll(num); }

int get_points(Board* board, bitboard pieces) {
  int points = 0;
  points += count(board->pawns & pieces) * 10;
  points += count((board->bishops | board->knights) & pieces) * 30;
  points += count(board->rooks & pieces) * 50;
  points += count(board->rooks & board->bishops & pieces) * 10;
  return points;
}

int eval(Board* board) {
  return get_points(board, board->white_pieces) -
         get_points(board, board->black_pieces);
}