#include "ChessBoardAPI/src/move.hpp"
#include "ChessBoardAPI/src/board.hpp"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

vector<string> split(string s) {
		vector<string> ret;
		int curr = 0;
		int pos;
		while(curr < s.size() &&(pos = s.find(" ", curr)) != string::npos) {
				ret.push_back(s.substr(curr, pos));
				curr = pos+1;
		}
		return ret;
}

Move next_move(Board board, int depth) {
		throw invalid_argument("Not yet implemented.");
}

int main() {
		cout << "Hello World!" << endl;
		Board board;
		start_board(&board);
		while(true) {
				string line;
				getline(cin, line);
				vector<string> spl = split(line);
				cout << "recieve "  << line << endl;
						if(spl[0] == "uci") {
								cout << "id name dominic cool engine" << endl;
								cout << "id author dominic" << endl;
								cout << "option name MultiPV type spin default 1 min 1 max 500" << endl;
								cout << "uciok" << endl;
						}
						else if (spl[0] == "quit") 
								return 0;
						else if(spl[0] == "isready")
								cout << "readyok" << endl;
						
						if ( spl[0] == "ucinewgame") start_board(&board); 

						if (spl[0] == "go") {
								Move best_move = next_move(board, 2);
								char move_str[5];
								print_move(move_str, &best_move);
								cout << move_str << " idk" << endl;
						} else cout << "unknown " << line << endl;
		}
}
