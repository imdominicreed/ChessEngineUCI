#include "tt.hpp"
using namespace std;

void Entry::save(unsigned long long key, int score, int depth, Move move) {
  // racy
  this->key = key;
  this->depth = depth;
  this->score = score;
  this->move = move;
}

TranspositionTable::TranspositionTable() {
  Board b = {};
  table.resize(100000007, {0, INVALID, -1});
}
void TranspositionTable::save(Board* b, int score, int depth, Move move) {
  Entry* e = &table[b->key % table.size()];
  e->save(b->key, score, depth, move);
}
string print_vector(vector<Move> moves) {
  string str;
  for (Move m : moves) {
    str += m.toString() + " ";
  }
  return str;
}

Entry TranspositionTable::probe(Board* b, int depth) {
  Entry entry = table[b->key % table.size()];
  if (entry.key != b->key || depth > entry.depth) return {0, INVALID, -1};
  return entry;
}
