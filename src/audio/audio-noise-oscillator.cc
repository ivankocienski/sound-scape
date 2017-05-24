
#include <stdlib.h>

#include "audio-noise-oscillator.hh"

AudioNoiseOscillator::AudioNoiseOscillator(int sr) : AudioOscillatorBase(sr) {
}

float AudioNoiseOscillator::next_value() {

  float r = 1.0 - 2.0 * ((float)rand() / (float)RAND_MAX);

  return r * m_volume;
}

