#include "tt.hpp"
using namespace std;

void Entry::save(long long key, int score, int depth, Board board) {
  // racy
  this->depth = depth;
  this->score = score;
  this->key = key;
  this->board = board;
}

TranspositionTable::TranspositionTable() {
  Board b = {};
  table.resize(100007, {0, INVALID, -1, b});
}
void TranspositionTable::save(Board* b, int score, int depth) {
  Entry* e = &table[b->key % table.size()];
  e->save(b->key, score, depth, *b);
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
  if (entry.key == b->key && *b != entry.board && entry.score != INVALID) {
    cerr << "Error Occured with Key!!\nKey: " << endl;
    //  << "\n "
    //  //  << print_vector(entry.board.move_list) << "\n"
    //  << entry.board.toString()
    //  << '\n'
    //  //  << print_vector(b->move_list) << "\n"
    //  << b->toString() << '\n';
  }
  if (entry.key != b->key || depth > entry.depth) return INVALID;
  return entry.score;
}
