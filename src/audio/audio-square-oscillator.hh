
#pragma once

#include <vector>

#include "audio-oscillator-base.hh"

class AudioSquareOscillator : public AudioOscillatorBase {
private:

  int m_half_wave;

public:

  AudioSquareOscillator(int);

  float next_value();
};

