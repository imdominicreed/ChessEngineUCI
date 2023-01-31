#include <sys/time.h>

#include <engine.hpp>

int64_t end_time;
bool out;

int64_t get_time() {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}
void set_move_time(int time) {
  end_time = get_time() + time;
  out = false;
}
void set_remaining_time(int time, int num_moves) {
  end_time = get_time() + (time / 40);
  std::cerr << "time " << time / 40 << std::endl;
  out = false;
}

void check_time() { out = get_time() >= end_time; }

bool out_of_time() { return out; }