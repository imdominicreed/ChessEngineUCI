#include "tt.hpp"
using namespace std;

int overwrite;

void Entry::save(uint64_t key, int score, uint8_t depth, Move move,
                 NodeType type) {
  uint64_t data = (score & 0x00000000FFFFFFFF);
  data |= (((uint64_t)depth) << 32) & 0x00ff00000000;
  data |= (((uint64_t)type) << 40) & 0xff0000000000;
  data |= ((uint64_t)move) << 48;

  this->xor_key = data ^ key;
  this->data = data;
}

TranspositionTable::TranspositionTable() {
  table = new Entry*[SIZE];
  for (int i = 0; i < SIZE; i++) {
    table[i] = new Entry[CLUSTER_SIZE];
    for (int j = 0; j < CLUSTER_SIZE; j++) table[i][j] = INVALID_ENTRY;
  }
}
TranspositionTable::~TranspositionTable() { delete table; }
void TranspositionTable::save(Board* b, int score, uint16_t depth, Move move,
                              NodeType type) {
  Entry* cluster = table[b->key % SIZE];
  int i = 0;
  while (i < CLUSTER_SIZE) {
    if (cluster[i].depth() == INVALID_DEPTH || (cluster[i].key() == b->key)) {
      if (cluster[i].depth() == INVALID_DEPTH || cluster[i].depth() < depth)
        cluster[i].save(b->key, score, depth, move, type);
      return;
    }
    i++;
  }
  overwrite++;
  cluster[rand() % CLUSTER_SIZE].save(b->key, score, depth, move, type);
}

Entry TranspositionTable::probe(Board* b) {
  Entry* cluster = table[b->key % SIZE];

  int i = 0;
  while (i < CLUSTER_SIZE) {
    Entry entry = cluster[i++];
    if (entry.key() == b->key) return entry;
  }

  return INVALID_ENTRY;
}

void TranspositionTable::clear() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < CLUSTER_SIZE; j++) table[i][j] = INVALID_ENTRY;
  }
}
