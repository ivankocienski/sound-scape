
#include <iostream>

#include <cstdarg>
#include <ctime>
#include <cstdlib>

#include "misc.hh"

static bool debug_enabled = false;
static char debug_buffer[1024];

using std::cerr;
using std::endl;

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

void set_debug(bool en) {
  debug_enabled = en;
}

void debug(const char *format, ...) {

  if(debug_enabled) {
    va_list args;
    va_start(args, format);
    vsnprintf(debug_buffer, 1024, format, args);
    va_end(args);

    cerr << "debug: " << debug_buffer << endl;
  }
}
