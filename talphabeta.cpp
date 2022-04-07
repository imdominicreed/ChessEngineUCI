#include "ChessBoardAPI/src/board.hpp"
#include "engine.hpp"
#include <fstream>
#include <stdint.h>
#include "ChessBoardAPI/src/move.hpp"
#include "ChessBoardAPI/src/move_gen.hpp"
#include "eval.hpp"
#include <iostream>
#include <algorithm>    // std::max

using namespace std;

int alphabetat(Board* board, int depth, int alpha, int beta, TranspositionTable* t) {
	if(depth == 0) return eval(board);
	Move move_list[256];
	int num_moves = get_move_list(board, move_list);
	if(num_moves == 0) return 0;
	if(num_moves == -1) return (board->white?1:-1)*checkmate;
	int tt_score = t->getScore(board, depth);
	if(tt_score != INVALID) {
		return tt_score;
	}

	if(board->white) {
		int value = INT32_MIN;
		for(int i = 0; i < num_moves; i++) {
			Board b = do_move(&move_list[i], *board);
			value = std::max(value, alphabetat(&b, depth-1, alpha, beta, t));
			alpha = std::max(alpha, value);
			if(value >= beta) break;
		}
		t->save(board, value, depth);
		return value;
	} else {
		int value = INT32_MAX;
		for(int i = 0; i < num_moves; i++) {
			Board b = do_move(&move_list[i], *board);
			value = std::min(value, alphabetat(&b, depth-1, alpha, beta, t));
			beta = std::min(beta, value);
			if(value <= alpha) break;
		}
		t->save(board, value, depth);
		return value;
	}
}

Move best_move_alphabeta_transpose(Board* board, int depth) {
	int color = board->white ? 1 : -1;
	TranspositionTable t;
	Move move_list[256];
	int best = INT32_MIN;
	Move best_move;
	int num_moves = get_move_list(board, move_list);
	for(int i = 0; i < num_moves; i++) {
		Board b = do_move(&move_list[i], *board);
		int move_score = color * alphabetat(&b, depth -1, INT32_MIN, INT32_MAX, &t);
		if (move_score > best) {
			best_move = move_list[i];	
			best = move_score;
		}
	}
	return best_move;
}
