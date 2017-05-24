
#pragma once

#include <boost/shared_ptr.hpp>

class AudioOscillatorBase {
protected:

  int   m_sample_rate;
  int   m_pitch;
  float m_volume;
  int   m_position;
  
public:
  
  AudioOscillatorBase(int);

  void configure( int, float );
  int sample_rate();

  virtual float next_value();
};

typedef boost::shared_ptr<AudioOscillatorBase> audio_oscillator_ptr;
