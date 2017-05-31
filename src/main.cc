#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <csignal>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "audio/audio.hh"
#include "misc.hh"
#include "track.hh"
#include "config.hh"

using namespace std; 
using namespace audio;
namespace fs = boost::filesystem;

static bool run_loop = true;

static void sigint_handler(int signum) {

  cout << "got interrupt signal, stopping" << endl;

  run_loop = false;
}

void scan_for_audio_tracks(const string &dir_path, list<Track> & tracks) {
  
  fs::path dir(fs::system_complete(dir_path)); 
  list<Sample> segments;

  fs::directory_iterator end_iter;
  for (fs::directory_iterator dir_itr(dir); dir_itr != end_iter; ++dir_itr) {

    if(!fs::is_regular_file(dir_itr->status())) continue;

    string file_name = dir_itr->path().filename().native();

    size_t last_dot = file_name.find_last_of(".");
    if(last_dot ==  string::npos) continue;

    size_t end_wav = file_name.find(".flac", last_dot);
    if(end_wav ==  string::npos) continue;

    Track track;
    track.load(dir_itr->path().c_str());

    if(track.is_empty()) continue;

    track.soften();
    tracks.push_back(track);
  }
}

int main( int argc, char** argv ) {

  srand(time(NULL));

  config_parse_args(argc, argv);

  if(config_wants_help) {
    config_show_help();
    return 0;
  }

  if(!config_is_valid()) {
    cerr << "Invalid configuration provided" << endl;
    return -1;
  }



  try {
    list<Track> tracks;

    for( auto & it : config_data_sources ) 
      scan_for_audio_tracks(it, tracks);

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
