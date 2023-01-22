#pragma once

#include "../ChessBoardAPI/board/board.hpp"
/*  Pawn,
  Bishop,
  Queen,
  Rook,
  King,
  Knight,
  None*/
const int value[] = {1, 3, 3, 5, 1000, 9, 0};

void move_sort(Move* start, Move* end, Board* board);