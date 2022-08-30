#include <stdint.h>

#include <algorithm>  // std::max
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <thread>
#include <utility>
#include <vector>

#include "ChessBoardAPI/src/board.hpp"
#include "ChessBoardAPI/src/move.hpp"
#include "ChessBoardAPI/src/move_gen.hpp"
#include "engine.hpp"
#include "eval.hpp"
using namespace std;

void runminmax(Board board, int depth, std::promise<int> p, Move m) {
  p.set_value(minmax(&board, depth));
}

Move best_move_parallelminmax(Board* board, int depth) {
  int color = board->white ? 1 : -1;
  Move move_list[256];
  int best = INT32_MIN;
  Move best_move;
  int num_moves = get_move_list(board, move_list);
  vector<std::thread> threads;
  vector<std::future<int>> futures;
  for (int i = 0; i < num_moves; i++) {
    Board b = do_move(&move_list[i], *board);
    std::promise<int> p;
    futures.push_back(move(p.get_future()));
    std::thread t(runminmax, b, depth - 1, std::move(p), move_list[i]);
    threads.push_back(std::move(t));
  }
  cerr << "\nNew Move Sequence\n";
  // cerr << "Num moves: " << num_moves << " num futures: " << futures.size() <<
  // endl;
  for (int i = 0; i < num_moves; i++) {
    threads[i].join();
    int data = futures[i].get();
    int eval = data * color;
    char first[5];
    print_move(first, &move_list[i]);
    cerr << "Evaluation: " << eval << " Move: " << first << " Line: ";
    cerr << endl;
    if (eval > best) {
      best = eval;
      best_move = move_list[i];
    }
  }
  return best_move;
}
