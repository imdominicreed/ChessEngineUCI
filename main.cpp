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
  // if (argc != 2) {
  //   printf(
  //       "Pass args:\n"
  //       "pminmax\nminmax\nalphabeta\npalphabeta\ntalphabeta\ntpalphabeta\n");
  //   return -1;
  // }
  // if (!strcmp("pminmax", argv[1]))
  //   best_move = best_move_parallelminmax;
  // else if (!strcmp("minmax", argv[1]))
  //   best_move = best_move_minmax;
  // else if (!strcmp("alphabeta", argv[1]))
  //   best_move = best_move_alphabeta;
  // else if (!strcmp("palphabeta", argv[1]))
  //   best_move = best_move_parallelalphabeta;
  // else if (!strcmp("talphabeta", argv[1]))
  //   best_move = best_move_alphabeta_transpose;
  // else if (!strcmp("tpalphabeta", argv[1]))
  //   best_move = best_move_alphabeta_transpose_parallel;
  // else {
  //   printf(
  //       "Pass args:\n"
  //       "pminmax\nminmax\nalphabeta\npalphabeta\ntalphabeta\ntpalphabeta\n");
  //   return -1;
  // }

  best_move = best_move_alphabeta_transpose_parallel;
  Board board;
  board.startBoard();
  string line;
  while (getline(cin, line)) {
    cerr << "recieve " << line << endl;
    vector<string> spl = split(line);
    cerr << "size: " << spl.size() << endl;
    if (!spl.size()) continue;
    if (spl[0] == "uci") {
      cout << "id name dominic engine" << endl;
      cout << "id author dominic\n" << endl;
      cout << "uciok" << endl;
      cerr << "id name dominic engine" << endl;
      cerr << "id author dominic\n" << endl;
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
        cerr << "Move: " << m.toString() << endl;
        board = board.doMove(&m);
      }
    } else if (spl[0] == "go") {
      Move best_move = next_move(board, 7);
      cout << "bestmove " << best_move.toString() << endl;
    } else
      cerr << "unknown " << line << endl;
  }
}
