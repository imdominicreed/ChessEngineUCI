#include "ChessBoardAPI/src/board.hpp"
#include <fstream>
#include <stdint.h>
#include "ChessBoardAPI/src/move.hpp"
#include "ChessBoardAPI/src/move_gen.hpp"
#include "eval.hpp"
#include <algorithm>    // std::max

int checkmate = 10000000;
int minmax(Board* board, int depth) {
	if(depth == 0) return eval(board);
	Move move_list[256];
	int num_moves = get_move_list(board, move_list);
	if(num_moves == 0) return 0;
	if(num_moves == -1) return (board->white?1:-1)*checkmate;
	int best = board->white ? INT32_MIN : INT32_MAX;
	for(int i = 0; i < num_moves; i++) {
		Board b = do_move(&move_list[i], *board);
		int score = minmax(&b, depth-1);
		if(board->white) best = std::max(best, score);
		else best = std::min(best, score);
	}
	return best;
}

Move best_move(Board* board) {
	int color = board->white ? 1 : -1;
	std::ofstream file;
	file.open("eval");	
	file << "curr_eval: " << eval(board) << "\n";
	Move move_list[256];
	int best = INT32_MIN;
	Move best_move;
	int num_moves = get_move_list(board, move_list);
	for(int i = 0; i < num_moves; i++) {
		Board b = do_move(&move_list[i], *board);
		int move_score = color * minmax(&b, 4);
		if (move_score > best) {
			best_move = move_list[i];	
			best = move_score;
		}
	}
	char move_str[5];
	print_move(move_str, &best_move);
	file << move_str << " " << best << "\n";
	return best_move;
}
