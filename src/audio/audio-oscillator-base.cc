
#include "audio-oscillator-base.hh"

AudioOscillatorBase::AudioOscillatorBase(int sr) {
  m_sample_rate = sr;
  m_pitch       = 0;
  m_volume      = 0;
  m_position    = 0;
}

void AudioOscillatorBase::configure( int p, float v ) { 
  m_pitch  = p;
  m_volume = v;
}

int AudioOscillatorBase::sample_rate() {
  return m_sample_rate;
}

float AudioOscillatorBase::next_value() { return 0; }

