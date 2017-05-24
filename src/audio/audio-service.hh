
#pragma once

#include <vector>

#include <portaudio.h>

#include "audio-channel.hh"
#include "audio-pattern.hh"
#include "audio-oscillator-base.hh"

class AudioService {
private:

  PaStream *m_stream;

  float m_volume;
  bool  m_paused;
  
  std::vector<AudioChannel> m_channels;

  // oscillators
  std::vector< audio_oscillator_ptr > m_oscillators;

  // pattern buffers
  std::vector< audio_pattern_buffer_t > m_patterns;

public:

  enum { // waveform
    WF_SINE,
    WF_SQUARE,
    WF_NOISE,
    WF_SAWTOOTH,
    WF_TRIANGLE,
    WF_SILENCE
  };

  static const int c_sample_rate;
  static const int c_num_channels;
  static const int c_data_type;

  AudioService();
  ~AudioService();

  void init(); 
  void start(); 
  void stop();

  bool is_busy();

  void play(int, int);
  void loop(int, int);
  void set_pattern( int, AudioPattern& );
  void set_volume(float);
  void pause(bool);

  const audio_oscillator_ptr & oscillator(int);
  AudioChannel& channel(int);
  
  friend int audio_callback( const void*, void*, unsigned long, PaStreamCallbackTimeInfo*, const PaStreamCallbackFlags, void*);

  int callback( float*, unsigned long);
};

