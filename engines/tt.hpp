#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include <vector>

#include "../ChessBoardAPI/board/board.hpp"

const int INVALID = INT32_MIN + 1;

struct Entry {
  unsigned long long key;
  int score;
  int depth;
  Move move;
  void save(unsigned long long key, int score, int depth, Move move);
};
class TranspositionTable {
 private:
  std::vector<Entry> table;

 public:
  TranspositionTable();
  Entry probe(Board* b, int depth);
  void save(Board* b, int score, int depth, Move move);
};
