
#pragma once

#include <vector>

#include "audio-oscillator-base.hh"

class AudioSineOscillator : public AudioOscillatorBase {
private:

  std::vector<float> m_wave;

public:

  AudioSineOscillator(int);

  float next_value();
};

