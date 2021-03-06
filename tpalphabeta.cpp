#include "ChessBoardAPI/src/board.hpp"
#include <iostream>
#include <future>
#include <fstream>
#include "engine.hpp"
#include <stdint.h>
#include "ChessBoardAPI/src/move.hpp"
#include "ChessBoardAPI/src/move_gen.hpp"
#include "eval.hpp"
#include <algorithm>    // std::max
#include <vector>
#include <thread>
#include <utility>
#include <vector>
#include <sstream>
using namespace std;




void runalphabetat(Board board, int depth, std::promise<int> p, Move m, TranspositionTable* t) {
	p.set_value(alphabetat(&board, depth, INT32_MIN, INT32_MAX, t));
}

Move best_move_alphabeta_transpose_parallel(Board* board, int depth) {
	int color = board->white ? 1 : -1;
	Move move_list[256];
	TranspositionTable tr;
	int best = INT32_MIN;
	Move best_move;
	int num_moves = get_move_list(board, move_list);
	vector<std::thread> threads;
	vector<std::future<int>> futures;
	for(int i = 0; i < num_moves; i++) {
		Board b = do_move(&move_list[i], *board);
		std::promise<int> p;	
		futures.push_back(move(p.get_future()));
		std::thread t(runalphabetat, b, depth -1, std::move(p), move_list[i], &tr);
		threads.push_back(std::move(t));
	}
	for(int i = 0; i < num_moves; i++) {
		threads[i].join();
		int data = futures[i].get();
		int eval = data * color;
		if(eval > best) {
			best = eval;
			best_move = move_list[i];
		}
	}
	return best_move;
}
