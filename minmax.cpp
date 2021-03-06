#include "ChessBoardAPI/src/board.hpp"
#include "engine.hpp"
#include <fstream>
#include <stdint.h>
#include "ChessBoardAPI/src/move.hpp"
#include "ChessBoardAPI/src/move_gen.hpp"
#include "eval.hpp"
#include <algorithm>    // std::max

int minmax(Board* board, int depth) {
	if(depth == 0) return eval(board); //base case, leaf node: just return eval of board
	Move move_list[256];
	int num_moves = get_move_list(board, move_list);
	if(num_moves == 0) return 0; // this is a draw so we just return 0
	if(num_moves == -1) return (board->white?1:-1)*checkmate; //returns big/small value if checkmate
	int best = board->white ? INT32_MIN : INT32_MAX; //set to some value that will be changed
	for(int i = 0; i < num_moves; i++) {
		Board b = do_move(&move_list[i], *board); //does move
		int score = minmax(&b, depth-1); //recursive minmax
		if(board->white) best = std::max(best, score); //get max if white
		else best = std::min(best, score); //get min if black
	}
	return best;
}

Move best_move_minmax(Board* board, int depth) {
	int color = board->white ? 1 : -1;
	Move move_list[256];
	int best = INT32_MIN;
	Move best_move;
	int num_moves = get_move_list(board, move_list);
	for(int i = 0; i < num_moves; i++) {
		Board b = do_move(&move_list[i], *board);
		int move_score = color * minmax(&b, depth - 1);
		if (move_score > best) {
			best_move = move_list[i];	
			best = move_score;
		}
	}
	char move_str[5];
	print_move(move_str, &best_move);
	return best_move;
}
