
#include <math.h>

#include "audio-square-oscillator.hh"

AudioSquareOscillator::AudioSquareOscillator(int sr) : AudioOscillatorBase(sr) {
  m_half_wave = m_sample_rate / 2;
}

float AudioSquareOscillator::next_value() {

  m_position += m_pitch;
  if(m_position > m_sample_rate) m_position -= m_sample_rate;

  return (m_position < m_half_wave) ? -m_volume : m_volume;
}

