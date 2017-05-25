#include <iostream>
#include <unistd.h>

#include "audio/audio.hh"

using namespace std; 
using namespace audio;

int main( int arch, char** argv ) {

  try {

    Service audio;
    audio.init();

    Sample sample; 
    sample.load("data/sample-00.wav");
    cout << "Using sample-00.wav" << endl; 

    audio.queue(sample);

    audio.start();

    sleep(10);

  } catch( const Exception &x ) {

    cerr << "error: (" << x.function() << ") " << x.message() << endl;
  } 

  return 0; 
}
