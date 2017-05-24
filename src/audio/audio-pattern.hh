
#pragma once

#include <list>

#include "audio-oscillator-base.hh"
#include "audio-common.hh"

class AudioPatternTone {
private:

  const audio_oscillator_ptr m_osc;
  float m_volume;
  int   m_pitch;
  float m_msec;

public:

  AudioPatternTone( const audio_oscillator_ptr, float, int, int );
  
  int sample_len();
  void render( audio_pattern_buffer_t&, int );
};

class AudioPattern {
private:

  typedef std::list<AudioPatternTone> tone_list_t;

  tone_list_t m_tones;

  int m_tones_length; 

public:

  AudioPattern();

  void beep( const audio_oscillator_ptr, float, int, int );

  void render( audio_pattern_buffer_t& );
  
};

