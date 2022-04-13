#include <ctime>
#include "tt.hpp"
#include <cstdlib>
#include <set>
#include "ChessBoardAPI/src/board.hpp"
#include <iostream>
using namespace std;

void Entry::save(long long key, int score, int depth) {
	//racy 
		this->depth = depth;
		this->score = score;
		this->key = key;
}

TranspositionTable::TranspositionTable() {
	Board b;
	table.resize(1000007, {0, INVALID, -1});
	
}
void TranspositionTable::save(Board* b, int score, int depth) {
	Entry* e = &table[b->key % table.size()];
	e->save(b->key, score, depth);
}



int TranspositionTable::getScore(Board* b, int depth) {
	Entry entry = table[b->key % table.size()];
	if(entry.key != b->key || depth > entry.depth) return INVALID;
	return entry.score;
}
