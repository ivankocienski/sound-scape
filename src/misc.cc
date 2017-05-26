
#include <ctime>
#include <cstdlib>

void ms_sleep(int dur) {
  struct timespec ts;

  ts.tv_sec = 0;
  ts.tv_nsec = 1000000 * dur;

  nanosleep(&ts, NULL);
}

float frand() {
  return float(rand()) / float(RAND_MAX);
}

float sfrand() {
  return 1.0 - 2.0 * frand();
}
