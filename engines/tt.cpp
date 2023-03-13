#include "tt.hpp"
using namespace std;

void Entry::save(uint64_t key, int score, uint8_t depth, Move move,
                 NodeType type, Board* b) {
  uint64_t data = (score & 0x00000000FFFFFFFF);
  data |= (((uint64_t)depth) << 32) & 0x00ff00000000;
  data |= (((uint64_t)type) << 40) & 0xff0000000000;
  data |= ((uint64_t)move) << 48;

  this->key = data ^ key;
  this->data = data;
  this->b = *b;

  assert(score == this->score());
  assert(depth == this->depth());
  assert(move == this->move());
  assert(type == this->node_type());
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
  if (e->depth() == INVALID_DEPTH || e->depth() < depth)
    e->save(b->key, score, depth, move, type, b);
}

Entry TranspositionTable::probe(Board* b) {
  Entry entry = table[b->key % SIZE];
  if ((entry.key ^ entry.data) != b->key) return INVALID_ENTRY;
  if (*b != entry.b)
    cout << b->toString() << endl << entry.b.toString() << endl;
  return entry;
}

void TranspositionTable::clear() {
  for (int i = 0; i < SIZE; i++) {
    table[i] = INVALID_ENTRY;
  }
}
