#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "ChessBoardAPI/board/board.hpp"
#include "ChessBoardAPI/move/move.hpp"
#include "engines/engine.hpp"
#include "eval/eval.hpp"

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
Move next_move(Board board, int depth) { return best_move(&board, depth); }

int main(int argc, char** argv) {
  int debug = 0;
  if (debug) {
    Board b = import_fen(
        "rnbqkbnr/pppp1ppp/8/8/4p3/N4N2/PPPPPPPP/R1BQKB1R w KQkq - 0 3");
    cerr << "Eval: " << eval(&b);

    return 0;
  }

  best_move = ids_split;
  Board board;
  board.startBoard();
  string line;
  while (getline(cin, line)) {
    cerr << "recieve " << line << endl;
    vector<string> spl = split(line);
    cerr << "size: " << spl.size() << endl;
    if (!spl.size()) continue;
    if (spl[0] == "uci") {
      cout << "id name ids_split engine" << endl;
      cout << "id author dominic\n" << endl;
      cout << "uciok" << endl;

      cerr << "uciok" << endl;

    } else if (spl[0] == "quit")
      return 0;
    else if (spl[0] == "isready")
      cout << "readyok" << endl;

    else if (spl[0] == "ucinewgame")
      board.startBoard();
    else if (spl[0] == "normalize")
      continue;  // not implemented
    else if (spl[0] == "position") {
      int start = 3;
      if (spl[1] == "fen") {
        stringstream ss;
        for (int i = 2; i < 8; i++) {
          ss << spl[i] << " ";
        }
        char fen[512];
        strcpy(fen, ss.str().c_str());
        board = import_fen(fen);
        start = 8;
      } else
        board.startBoard();
      for (int i = start; i < spl.size(); i++) {
        char arr[5];
        strcpy(arr, spl[i].c_str());
        Move m = board.moveFromStr(arr);
        // cerr << "Move: " << m.toString() << endl;
        board.doMove(m);
      }
    } else if (spl[0] == "go") {
      Move best_move = next_move(board, 5000);
      cout << "bestmove " << to_string(best_move) << endl;

    } else
      cerr << "unknown " << line << endl;
  }
}
