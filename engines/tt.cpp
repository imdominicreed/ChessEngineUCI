#include "tt.hpp"
using namespace std;

void Entry::save(uint32_t key, int score, uint16_t depth, Move move) {
  // racy
  this->key = key;
  this->depth = depth;
  this->score = score;
  this->move = move;
}

TranspositionTable::TranspositionTable() {
  cerr << "error here" << endl;
  for (int i = 0; i < SIZE; i++) {
    table[i] = {0, INVALID, 0, 0};
  }
}
void TranspositionTable::save(Board* b, int score, uint16_t depth, Move move) {
  Entry* e = &table[b->key % SIZE];
  if (depth < e->depth) e->save(b->key, score, depth, move);
}

Entry TranspositionTable::probe(Board* b) {
  Entry entry = table[b->key % SIZE];
  if (entry.key != b->key) return {0, INVALID, 0, 1};
  return entry;
}
