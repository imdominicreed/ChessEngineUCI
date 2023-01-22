#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include <vector>

#include "../ChessBoardAPI/board/board.hpp"

const int INVALID = INT32_MIN + 1;
const int SIZE = 1 << 20;

struct Entry {
  uint32_t key;
  int score;
  uint16_t depth;
  Move move;
  void save(uint32_t key, int score, uint16_t depth, Move move);
};
class TranspositionTable {
 private:
  Entry *table;

 public:
  TranspositionTable();
  ~TranspositionTable();
  Entry probe(Board* b);
  void save(Board* b, int score, uint16_t depth, Move move);
};
