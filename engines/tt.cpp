#include "tt.hpp"
using namespace std;

void Entry::save(unsigned long long key, int score, int depth) {
  // racy
  this->key = key;
  this->depth = depth;
  this->score = score;
}

TranspositionTable::TranspositionTable() {
  Board b = {};
  table.resize(100000007, {0, INVALID, -1});
}
void TranspositionTable::save(Board* b, int score, int depth) {
  Entry* e = &table[b->key % table.size()];
  e->save(b->key, score, depth);
}
string print_vector(vector<Move> moves) {
  string str;
  for (Move m : moves) {
    str += m.toString() + " ";
  }
  return str;
}

int TranspositionTable::getScore(Board* b, int depth) {
  Entry entry = table[b->key % table.size()];
  if (entry.key != b->key || depth > entry.depth) return INVALID;
  return entry.score;
}
