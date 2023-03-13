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

int run_perft(Board* b, int depth) {
  Move move_list[256];
  Move* start = move_list;
  Move* end = b->getMoveList(move_list);

  if (end == nullptr) return 0;
  if (depth == 1) {
    int legal_moves = 0;
    while (start != end) {
      UndoMove undo = b->doMove(*start);
      if (!b->inCheck()) legal_moves++;
      b->undoMove(undo);
      start++;
    }
    return legal_moves;
  }

  Entry e = tt.probe(b);
  if (e.depth() == depth) {
    return e.score();
  }

  int states = 0;
  while (start != end) {
    UndoMove undo = b->doMove(*start);
    states += run_perft(b, depth - 1);
    b->undoMove(undo);
    start++;
  }

  tt.save(b, states, depth, 0, NodeType::PV);

  return states;
}

void perft(Board* b, int depth) {
  tt.clear();
  Move move_list[256];
  Move* start = move_list;
  Move* end = b->getMoveList(move_list);

  if (end == nullptr) return;
  int num_nodes = 0;

  while (start != end) {
    UndoMove undo = b->doMove(*start);
    if (b->inCheck()) {
      b->undoMove(undo);
      start++;
      continue;
    }
    int nodes = run_perft(b, depth - 1);
    b->undoMove(undo);

    num_nodes += nodes;
    cout << to_string(*start) << ": " << nodes << endl;
    start++;
  }

  cout << endl << "Nodes searched: " << num_nodes << endl;
}

int main(int argc, char** argv) {
  init_tables();
  int moves;
  Board board;
  board.startBoard();
  board = import_fen((char*)startFEN.c_str());
  string line;
  while (getline(cin, line)) {
    cerr << "recieve " << line << endl;
    stringstream ss(line);
    string command;
    string token;
    ss >> command;
    if (command == "uci") {
      cout << "id name 1 engine" << endl;
      cout << "id author dominic\n" << endl;
      cout << "uciok" << endl;
      cerr << "hash size " << MB_SIZE / (1024 * 1024) << endl;
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
      bool perf = false;
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
        else if ("perft" == token) {
          perft(&board, stoi(num));
          perf = true;
        }
      }
      if (perf) continue;
      if (move_time != -1)
        set_move_time(move_time);
      else
        set_remaining_time(board.turn == WHITE ? wtime : btime, moves);
      ids_split(&board);
    } else
      cerr << "unknown " << line << endl;
  }
}
