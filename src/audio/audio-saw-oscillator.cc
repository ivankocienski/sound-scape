
#include <math.h>

#include "audio-saw-oscillator.hh"

AudioSawOscillator::AudioSawOscillator(int sr) : AudioOscillatorBase(sr) {
}

float AudioSawOscillator::next_value() {

  float v = 1.0 - 2.0 * (float)m_position / (float)m_sample_rate;

  m_position += m_pitch;
  if(m_position > m_sample_rate) m_position -= m_sample_rate;

  return v * m_volume;
}

