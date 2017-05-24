
#pragma once

#include <vector>

#include "audio-oscillator-base.hh"

class AudioSawOscillator : public AudioOscillatorBase {
private:

public:

  AudioSawOscillator(int);

  float next_value();
};

