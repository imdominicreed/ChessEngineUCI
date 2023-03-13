#include <sys/time.h>

#include <engine.hpp>

int64_t end_time;
int64_t start_time;
bool out;

int64_t get_time() {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}
void set_move_time(int time) {
  start_time = get_time();
  end_time = start_time + time;
  out = false;
}
void set_remaining_time(int time, int num_moves) { set_move_time(time / 40); }

int64_t get_time_searched() { return get_time() - start_time; }

void check_time() { out = get_time() >= end_time; }

bool out_of_time() { return out; }