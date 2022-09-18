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
  void save(long long key, int score, int depth);
};
class TranspositionTable {
 private:
  std::vector<Entry> table;

 public:
  TranspositionTable();
  int getScore(Board* b, int depth);
  void save(Board* b, int score, int depth);
};
