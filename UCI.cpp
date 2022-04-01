#include "ChessBoardAPI/src/move.hpp"
#include <utility>
#include "eval.hpp"
#include "ChessBoardAPI/src/board.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstring>
#include "engine.hpp"

using namespace std;
vector<string> split(string s) {
	vector<string> ret;
	stringstream ss(s);
	string word;
	while (ss >> word) {
		ret.push_back(word);
	}
	return ret;
}
ftype best_move;
Move next_move(Board board, int depth) {
	return best_move(&board, depth);
}


int main(int argc, char** argv) {
	if(argc != 2) return 0;
	if(!strcmp("pminmax", argv[1])) best_move = best_move_parallelminmax;
	else if(!strcmp("minmax", argv[1])) best_move = best_move_minmax;
	else return 0;
	Board board;
	start_board(&board);
	string line;
	while(getline(cin, line)) {
		vector<string> spl = split(line);
		cerr << "recieve "  << line << endl;
		cerr << "size: " << spl.size() << endl;
		if(!spl.size()) continue;
		if(spl[0] == "uci") {
			cout << "id name dominic engine" << endl;
			cout << "id author dominic\n" << endl;
			cout << "uciok" << endl;
			cerr << "id name dominic engine" << endl;
			cerr << "id author dominic\n" << endl;
			cerr << "uciok" << endl;

		}
		else if (spl[0] == "quit") 
			return 0;
		else if(spl[0] == "isready")
			cout << "readyok" << endl;

		else if ( spl[0] == "ucinewgame") start_board(&board); 
		else if (spl[0] == "normalize") continue; // not implemented
		else if(spl[0] == "position") {
			int start = 3;
			if (spl[1] == "fen") {
				stringstream ss;
				for(int i = 2; i < 8; i++) {
					ss << spl[i] << " ";
				}
				char fen[512];	
				strcpy(fen, ss.str().c_str());
				Board* b = import_fen(fen);
				board = *b;
				free(b);
				start = 8;
			} else start_board(&board);
			for(int i = start; i < spl.size(); i++) {
				char arr[5];
				strcpy(arr, spl[i].c_str());
				Move m = move_from_str(board, arr);
				board = do_move(&m, board);
			}
		} else if (spl[0] == "go") {
			Move best_move = next_move(board, 5);
			char move_str[5];
			print_move(move_str, &best_move);
			cout << "bestmove " << move_str << endl;
		} else cerr << "unknown " << line << endl;
	} 
}
