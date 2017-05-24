
#pragma once

#include <vector>

#include "audio-oscillator-base.hh"

class AudioTriangleOscillator : public AudioOscillatorBase {
private:

  int m_half_wave;
  
public:

  AudioTriangleOscillator(int);

  float next_value();
};

