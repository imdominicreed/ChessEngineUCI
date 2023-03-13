#include "tt.hpp"
using namespace std;

int overwrite;

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
  Cluster& clust = table[b->key % SIZE];
  int i = 0;
  while (i < CLUSTER_SIZE) {
    if (clust.line[i].depth() == INVALID_DEPTH) {
      clust.line[i].save(b->key, score, depth, move, type);
      return;
    }
  }
  clust.line[rand() % 6].save(b->key, score, depth, move, type);
}

Entry TranspositionTable::probe(Board* b) {
  Cluster& clust = table[b->key % SIZE];

  int i = 0;
  while (i < CLUSTER_SIZE) {
    Entry& entry = clust.line[i++];
    if ((entry.key ^ entry.data) == b->key) return entry;
  }

  return INVALID_ENTRY;
}

void TranspositionTable::clear() {
  for (int i = 0; i < SIZE; i++) {
    table[i] = INVALID_ENTRY;
  }
}
