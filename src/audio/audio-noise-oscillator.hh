
#pragma once

#include <vector>

#include "audio-oscillator-base.hh"

class AudioNoiseOscillator : public AudioOscillatorBase {
private:

public:

  AudioNoiseOscillator(int);

  float next_value();
};

