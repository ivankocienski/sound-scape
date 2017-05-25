#include <iostream>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "audio/audio.hh"

using namespace std; 
using namespace audio;

void ms_sleep(int dur) {
  struct timespec ts;

  ts.tv_sec = 0;
  ts.tv_nsec = 1000000 * dur;

  nanosleep(&ts, NULL);
}

int main( int arch, char** argv ) {

  srand(time(NULL));

  try {

    vector<Sample> samples;
    { 
      Sample sample; 
      sample.load("data/sample-01.wav");
      cout << "Using sample-00.wav" << endl; 

      list<Sample> segments = sample.segment(100); 
      segments.remove_if( [](const Sample &sample) { return !sample.is_loud(0.1); } );

      samples.assign(segments.begin(), segments.end()); 

      for(auto it : samples)
        it.soften(20);
    }

    cout << "We have " << samples.size() << " mini-samples" << endl;

    Service audio;
    audio.init();

    int count = 500; // samples.size() - 1;
    while(count) {
      //audio.queue(samples[count]);
      audio.queue(samples[rand() % samples.size()]);

      count--;
    }

    audio.start();

    sleep(10);

  } catch( const Exception &x ) {

    cerr << "error: (" << x.function() << ") " << x.message() << endl;
  } 

  return 0; 
}
