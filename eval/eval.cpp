#pragma once

#include "eval.hpp"

#include <iostream>

// pawn positional score
const int pawn_score[64] = {
    90, 90, 90, 90,  90,  90, 90, 90, 30, 30, 30, 40, 40, 30, 30, 30,
    20, 20, 20, 30,  30,  30, 20, 20, 10, 10, 10, 20, 20, 10, 10, 10,
    5,  5,  10, 20,  20,  5,  5,  5,  0,  0,  0,  5,  5,  0,  0,  0,
    0,  0,  0,  -10, -10, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

// knight positional score
const int knight_score[64] = {
    -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0,   0,  10, 10, 0,  0,   -5,
    -5, 5,  20, 20, 20, 20, 5,  -5, -5, 10,  20, 30, 30, 20, 10,  -5,
    -5, 10, 20, 30, 30, 20, 10, -5, -5, 5,   20, 10, 10, 20, 5,   -5,
    -5, 0,  0,  0,  0,  0,  0,  -5, -5, -10, 0,  0,  0,  0,  -10, -5};

// bishop positional score
const int bishop_score[64] = {0,  0,  0,  0,  0, 0, 0,   0,  0,  0,   0,  0,  0,
                              0,  0,  0,  0,  0, 0, 10,  10, 0,  0,   0,  0,  0,
                              10, 20, 20, 10, 0, 0, 0,   0,  10, 20,  20, 10, 0,
                              0,  0,  10, 0,  0, 0, 0,   10, 0,  0,   30, 0,  0,
                              0,  0,  30, 0,  0, 0, -10, 0,  0,  -10, 0,  0

};

// rook positional score
const int rook_score[64] = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
                            50, 50, 50, 0,  0,  10, 20, 20, 10, 0,  0,  0,  0,
                            10, 20, 20, 10, 0,  0,  0,  0,  10, 20, 20, 10, 0,
                            0,  0,  0,  10, 20, 20, 10, 0,  0,  0,  0,  10, 20,
                            20, 10, 0,  0,  0,  0,  0,  20, 20, 0,  0,  0

};

// king positional score
const int king_score[64] = {0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  5,  5,  5,
                            5,  0,  0,  0,  5,  5,  10, 10, 5,   5,  0,  0,  5,
                            10, 20, 20, 10, 5,  0,  0,  5,  10,  20, 20, 10, 5,
                            0,  0,  0,  5,  10, 10, 5,  0,  0,   0,  5,  5,  -5,
                            -5, 0,  5,  0,  0,  0,  5,  0,  -15, 0,  10, 0};

// mirror positional score tables for opposite side
const int mirror_score[128] = {
    A8, B8, C8, D8, E8, F8, G8, H8, A7, B7, C7, D7, E7, F7, G7, H7,
    A6, B6, C6, D6, E6, F6, G6, H6, A5, B5, C5, D5, E5, F5, G5, H5,
    A4, B4, C4, D4, E4, F4, G4, H4, A3, B3, C3, D3, E3, F3, G3, H3,
    A2, B2, C2, D2, E2, F2, G2, H2, A1, B1, C1, D1, E1, F1, G1, H1,
};

int count(bitboard num) { return __builtin_popcountll(num); }

int eval(Board* board) {
  bitboard pieces = board->colorPiecesBB[WHITE] | board->colorPiecesBB[BLACK];
  int eval = 0;
  int sq;
  while (pieces) {
    POP_BSF(sq, pieces);
    if ((1ULL << sq) & board->colorPiecesBB[WHITE]) {
      switch (board->getPieceType(sq)) {
        case Pawn:
          eval += 100 + pawn_score[mirror_score[sq]];
          break;
        case Rook:
          eval += 500 + rook_score[mirror_score[sq]];
          break;
        case Bishop:
          eval += 300 + bishop_score[mirror_score[sq]];
          break;
        case Knight:
          eval += 300 + knight_score[mirror_score[sq]];
          break;
        case Queen:
          eval += 900;
          break;
        case King:
          eval += 100000 + king_score[mirror_score[sq]];
          break;
        default:
          break;
      }
    } else {
      switch (board->getPieceType(sq)) {
        case Pawn:
          eval -= 100 + pawn_score[sq];
          break;
        case Rook:
          eval -= 500 + rook_score[sq];
          break;
        case Bishop:
          eval -= 300 + bishop_score[sq];
          break;
        case Knight:
          eval -= 300 + knight_score[sq];
          break;
        case Queen:
          eval -= 900;
          break;
        case King:
          eval -= 100000 + king_score[sq];
          break;
        default:
          break;
      }
    }
  }
  return (board->turn == WHITE ? 1 : -1) * eval;
}
