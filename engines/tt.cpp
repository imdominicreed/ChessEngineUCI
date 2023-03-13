#include "tt.hpp"
using namespace std;

void Entry::save(uint64_t key, int score, uint8_t depth, Move move,
                 NodeType type) {
  uint64_t data = (score & 0x00000000FFFFFFFF);
  data |= (((uint64_t)depth) << 32) & 0x00ff00000000;
  data |= (((uint64_t)type) << 40) & 0xff0000000000;
  data |= ((uint64_t)move) << 48;

  this->key = data ^ key;
  this->data = data;
}

TranspositionTable::TranspositionTable() {
  table = new Entry[SIZE];
  for (int i = 0; i < SIZE; i++) {
    table[i] = INVALID_ENTRY;
  }
}
TranspositionTable::~TranspositionTable() { delete table; }
void TranspositionTable::save(Board* b, int score, uint16_t depth, Move move,
                              NodeType type) {
  Entry* e = &table[b->key % SIZE];
  e->save(b->key, score, depth, move, type);
}

Entry TranspositionTable::probe(Board* b) {
  Entry entry = table[b->key % SIZE];
  if ((entry.key ^ entry.data) != b->key) return INVALID_ENTRY;
  return entry;
}

void TranspositionTable::clear() {
  for (int i = 0; i < SIZE; i++) {
    table[i] = INVALID_ENTRY;
  }
}
