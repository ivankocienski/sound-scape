#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <csignal>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "audio/audio.hh"
#include "track.hh"

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

void scan_for_audio_tracks(list<Track> & tracks) {
  
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

    Track track;
    track.load(dir_itr->path().c_str());

    if(track.is_empty()) continue;

    track.soften();
    tracks.push_back(track);
  }

}

int main( int arch, char** argv ) {

  srand(time(NULL));

  try {
    list<Track> tracks;
    scan_for_audio_tracks(tracks);

    if(tracks.size() == 0) {
      cerr << "No samples found" << endl;
      return -1;
    }

    cout << "We have " << tracks.size() << " tracks" << endl;

    Service audio;

    audio.init();
    audio.start();

    signal(SIGINT, sigint_handler);
    
    while(run_loop) {
      audio.sweep_tidy();

      for(auto & it : tracks)
        it.tick(audio, 20);

      ms_sleep(20); 
    }


  } catch( const Exception &x ) {

    cerr << "error: (" << x.function() << ") " << x.message() << endl;
  } 

  cout << "bye now" << endl;

  return 0; 
}
