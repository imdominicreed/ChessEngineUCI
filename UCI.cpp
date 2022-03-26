#include "ChessBoardAPI/src/move.hpp"
#include "ChessBoardAPI/src/board.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

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

Move next_move(Board board, int depth) {
				throw invalid_argument("Not yet implemented.");
}

int main() {
				ofstream file;
				file.open("log.log");
				file<< "Hello World!" << endl;
				Board board;
				start_board(&board);
				string line;
				while(getline(cin, line)) {
								file << line << endl;
								vector<string> spl = split(line);
								file << "recieve "  << line << endl;
								file << "size: " << spl.size() << endl;
								if(!spl.size()) continue;
								if(spl[0] == "uci") {
												cout << "id name dominic engine" << endl;
												cout << "id author dominic\n" << endl;
												cout << "uciok" << endl;
												file << "id name dominic engine" << endl;
												file << "id author dominic\n" << endl;
												file << "uciok" << endl;

								}
								else if (spl[0] == "quit") 
												return 0;
								else if(spl[0] == "isready")
												cout << "readyok" << endl;

								else if ( spl[0] == "ucinewgame") start_board(&board); 
								else if (spl[0] == "normalize") continue; // not implemented
								else if(spl[0] == "position") continue;
								else if (spl[0] == "go") {
												//Move best_move = next_move(board, 2);
												cout << "info depth 1 seldepth 1 multipv 1 score cp 82 nodes 1000 nps 1 tbhits 0 time 2 pv" << endl;
												//print_move(move_str, &best_move);
												cout << "bestmove e2e4" << endl;
								} else file << "unknown " << line << endl;
				} 
}
