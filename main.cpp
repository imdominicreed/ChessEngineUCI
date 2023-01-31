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

const string startFEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main(int argc, char** argv) {
  init_tables();
  int moves;
  Board board;
  board.startBoard();
  string line;
  while (getline(cin, line)) {
    cerr << "recieve " << line << endl;
    stringstream ss(line);
    string command;
    string token;
    ss >> command;
    if (command == "uci") {
      cout << "id name ids_split engine" << endl;
      cout << "id author dominic\n" << endl;
      cout << "uciok" << endl;

      cerr << "uciok" << endl;

    } else if (command == "quit")
      return 0;
    else if (command == "isready")
      cout << "readyok" << endl;

    else if (command == "ucinewgame")
      board.startBoard();
    else if (command == "normalize")
      continue;  // not implemented
    else if (command == "position") {
      ss >> token;
      if (token == "fen") {
        stringstream fen_ss;

        while (ss >> token && token != "moves") {
          fen_ss << (token + " ");
        }
        char fen[512];
        strcpy(fen, fen_ss.str().c_str());
        board = import_fen(fen);
      } else {
        board = import_fen((char*)startFEN.c_str());
        ss >> token;
      }
      moves = 0;
      while (ss >> token) {
        char arr[5];
        moves++;
        strcpy(arr, token.c_str());
        Move m = board.moveFromStr(arr);
        board.doMove(m);
      }
      cerr << board.toString() << endl;
    } else if (command == "go") {
      int wtime, btime, winc, binc, move_time = -1;
      while (ss >> token) {
        string num;
        ss >> num;
        if ("wtime" == token)
          wtime = stoi(num);
        else if ("btime" == token)
          btime = stoi(num);
        else if ("winc" == token)
          winc = stoi(num);
        else if ("binc" == token)
          binc = stoi(num);
        else if ("movetime" == token)
          move_time = stoi(num);
      }
      if (move_time != -1)
        set_move_time(move_time);
      else
        set_remaining_time(board.turn == WHITE ? wtime : btime, moves);
      ids_split(&board);
    } else
      cerr << "unknown " << line << endl;
  }
}
