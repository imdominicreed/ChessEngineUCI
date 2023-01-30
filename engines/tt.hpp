#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>  // std::mutex, std::lock
#include <set>
#include <vector>

#include "../ChessBoardAPI/board/board.hpp"

const int SIZE = 1 << 20;
const uint64_t INVALID_DEPTH = 255;

enum class NodeType { PV = 0, Cut, All };

struct Entry {
  uint64_t key;
  uint64_t data;
  void save(uint64_t key, int score, uint8_t depth, Move move, NodeType type);
  inline int score() { return data & 0xFFFFFFFF; }
  inline int depth() { return ((data >> 32) & 0xFF); }
  inline Move move() { return (Move)(data >> 48); }
  inline NodeType node_type() { return (NodeType)((data >> 40) & 0xFF); };
};

const Entry INVALID_ENTRY = {0, INVALID_DEPTH << 32};

class TranspositionTable {
 private:
  Entry* table;

 public:
  TranspositionTable();
  ~TranspositionTable();
  Entry probe(Board* b);
  void save(Board* b, int score, uint16_t depth, Move move, NodeType type);
};
