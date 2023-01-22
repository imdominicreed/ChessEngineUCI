#include "move_picker.hpp"

void insertionSort(int* score, Move* array, int n) {
  for (int step = 1; step < n; step++) {
    int key = score[step];
    Move tmp = array[step];

    int j = step - 1;

    // Compare key with each element on the left of it until an element smaller
    // than it is found. For descending order, change key<array[j] to
    // key>array[j].
    while (score[j] < key && j >= 0) {
      array[j + 1] = array[j];
      score[j + 1] = score[j];
      --j;
    }
    array[j + 1] = tmp;
    score[j + 1] = key;
  }
}

void move_sort(Move* start, Move* end, Board* board) {
  int n = (end - start) / sizeof(Move);
  int score[n];
  for (int i = 0; i < n; i++) {
    PieceType victim = board->getPieceType(to(start[i]));
    if (victim == PieceType::None) {
      score[i] = -100000;
      continue;
    }
    PieceType aggressor = board->getPieceType(from(start[i]));
    score[i] = value[victim] - value[aggressor];
  }
  insertionSort(score, start, n);
}