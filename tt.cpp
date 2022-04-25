#include <ctime>
#include "tt.hpp"
#include <cstdlib>
#include <set>
#include "ChessBoardAPI/src/board.hpp"
#include <iostream>
using namespace std;

void Entry::save(long long key, int score, int depth, Board b) {
	//racy 
	this->depth = depth;
	this->score = score;
	this->key = key;
	this->b = b;	
}

TranspositionTable::TranspositionTable() {
	Board b;
	table.resize(1000007, {0, INVALID, -1});

}
void TranspositionTable::save(Board* b, int score, int depth) {
	Entry* e = &table[b->key % table.size()];
	e->save(b->key, score, depth, *b);
}



int TranspositionTable::getScore(Board* b, int depth) {
	Entry entry = table[b->key % table.size()];
	if(entry.key != b->key || depth > entry.depth) return INVALID;
	if(!b->equal(*b, entry.b)) {
		cerr << "STARD OF BUG \n";
		cerr << "board board" << endl;
		cerr << printBoard(b) << endl;
		cerr << "entry board" << endl;
		cerr << printBoard(&entry.b) << endl;
		cerr << "hash " << b->key << endl;
		cerr << "END OF BUG\n";
	}
	return entry.score;
}
