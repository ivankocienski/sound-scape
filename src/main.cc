#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <csignal>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "audio/audio.hh"

using namespace std; 
using namespace audio;
namespace fs = boost::filesystem;

static bool run_loop = true;

static void sigint_handler(int signum) {

  cout << "got interrupt signal, stopping" << endl;

  run_loop = false;
}

void ms_sleep(int dur) {
  struct timespec ts;

  ts.tv_sec = 0;
  ts.tv_nsec = 1000000 * dur;

  nanosleep(&ts, NULL);
}

void load_audio(list<Sample> & segments, const char *path) {

  Sample sample; 
  sample.load(path);
  cout << "loading " << path << endl; 

  sample.segment(segments, 100); 
}

void scan_for_audio_samples(vector<Sample> & samples) {
  
  fs::path dir(fs::system_complete("data/")); 
  list<Sample> segments;

  fs::directory_iterator end_iter;
  for (fs::directory_iterator dir_itr(dir); dir_itr != end_iter; ++dir_itr) {

    if(!fs::is_regular_file(dir_itr->status())) continue;

    string file_name = dir_itr->path().filename().native();
    cout << "file_name=" << file_name << endl;

    size_t last_dot = file_name.find_last_of(".");
    if(last_dot ==  string::npos) continue;

    size_t end_wav = file_name.find(".wav", last_dot);
    if(end_wav ==  string::npos) continue;

    load_audio(segments, dir_itr->path().c_str());
  }

  segments.remove_if( [](const Sample &sample) { return !sample.is_loud(0.1); } );

  samples.assign(segments.begin(), segments.end()); 

  for(auto it : samples)
    it.soften(20);
}

int main( int arch, char** argv ) {

  srand(time(NULL));

  try {
    vector<Sample> samples;
    scan_for_audio_samples(samples);

    if(samples.size() == 0) {
      cerr << "No samples found" << endl;
      return -1;
    }

    cout << "We have " << samples.size() << " mini-samples" << endl;

    Service audio;

    audio.init();
    audio.start();

    signal(SIGINT, sigint_handler);
    
    while(run_loop) {
      audio.sweep_tidy();

      audio.queue(samples[rand() % samples.size()]);

      ms_sleep(100); 
    }


  } catch( const Exception &x ) {

    cerr << "error: (" << x.function() << ") " << x.message() << endl;
  } 

  cout << "bye now" << endl;

  return 0; 
}
