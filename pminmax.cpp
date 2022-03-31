#include "ChessBoardAPI/src/board.hpp"
#include <iostream>
#include <future>
#include <fstream>
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
int checkmate = 10000000;
struct move_data {
	vector<Move>* best_line;
	int eval;
};
move_data minmax(Board board, int depth, Move m) {
	if(depth == 0) {
		//char mo[5];
		//print_move(mo, &m);
		//stringstream s;
		//s <<  "Eval: " << eval(&board) << " " << mo << "\n " << printBoard(&board) << endl;
		//cerr << s.str();


		return {new vector<Move>, eval(&board)};
	}
	Move move_list[256];
	int num_moves = get_move_list(&board, move_list);
	if(num_moves == 0) return {new vector<Move>, 0};
	if(num_moves == -1) {
		return {new vector<Move>, (board.white?1:-1)*checkmate};
	}
	Move best_move;
	move_data best = {NULL, board.white ? -checkmate-1 : checkmate+1};
	for(int i = 0; i < num_moves; i++) {
		Board b = do_move(&move_list[i], board);
		move_data score = minmax(b, depth-1, m);
		if ((board.white && score.eval > best.eval) || (!board.white && score.eval < best.eval)) {
			best = score;
			best_move = move_list[i];
		}
	}
	best.best_line->insert(best.best_line->begin(), best_move); 
	return best;
}

void runminmax(Board* board, int depth, std::promise<move_data> p, Move m) {
	p.set_value(minmax(*board, depth, m));
}

pair<Move, int> best_move(Board* board) {
	int color = board->white ? 1 : -1;
	Move move_list[256];
	int best = INT32_MIN;
	Move best_move;
	int num_moves = get_move_list(board, move_list);
	vector<std::thread> threads;
	vector<std::future<move_data>> futures;
	for(int i = 0; i < num_moves; i++) {
		Board b = do_move(&move_list[i], *board);
		std::promise<move_data> p;	
		char mo[5];
		print_move(mo, &move_list[i]);
		stringstream s;
		s <<  "Move: " << mo << "\n" << printBoard(&b) << endl;
		cerr << s.str();
		futures.push_back(move(p.get_future()));
		std::thread t(runminmax, &b, 0, std::move(p), move_list[i]);
		threads.push_back(std::move(t));
	}
	cerr << "\nNew Move Sequence\n";
	//cerr << "Num moves: " << num_moves << " num futures: " << futures.size() << endl;
	for(int i = 0; i < num_moves; i++) {
		threads[i].join();
		move_data data = futures[i].get();
		int eval = data.eval * color;
		char first[5];
		print_move(first, &move_list[i]);
		cerr << "Evaluation: " << eval << " Move: " << first << " Line: ";
		vector<Move> line = *data.best_line;
		for(Move m : line) {
			char move_str[5];
			print_move(move_str, &m);
			cerr << move_str << " ";
		}
		cerr << endl;
		if(eval > best) {
			best = eval;
			best_move = move_list[i];
		}
	}
	return {best_move, best};
}
