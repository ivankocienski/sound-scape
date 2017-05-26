#pragma once

#include "common.hh"
#include "exception.hh"
#include "sample.hh"

namespace audio {

class Service {
private:

  PaStream *m_stream;

  float m_volume;
  bool  m_paused;
  
  typedef struct playback_track_s { 
    float *sample_ptr;
    float volume;
    int sample_remaining; 

    playback_track_s() { 
      sample_ptr = NULL;
      sample_remaining = 0;
      volume = 1.0;
    }

    playback_track_s(Sample & sample, float v) {
      Sample::sample_info_t sample_info = sample.sample_info();

      sample_ptr = sample_info.data;
      sample_remaining = sample_info.data_length;
      if(v < 0) v = 0;
      if(v > 1) v = 1;
      volume = v;
    }

  } playback_track_t, *p_playback_track_t;

  std::list<playback_track_t> m_play_tracks;

public:

  static const int c_sample_rate;
  static const int c_num_channels;
  static const int c_data_type;

  Service();
  ~Service();

  void init(); 
  void start(); 
  void stop();

  bool is_busy();
  void sweep_tidy();

  void queue(Sample&, float);

  void set_volume(float);
  float volume();

  void set_paused(bool);
  bool paused();

  friend int audio_callback( 
      const void*,
      void*,
      unsigned long,
      PaStreamCallbackTimeInfo*,
      const PaStreamCallbackFlags,
      void*);

  int callback( float*, unsigned long);
};

} // namespace audio
