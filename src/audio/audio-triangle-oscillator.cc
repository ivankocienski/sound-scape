
#include <math.h>

#include "audio-triangle-oscillator.hh"

AudioTriangleOscillator::AudioTriangleOscillator(int sr) : AudioOscillatorBase(sr) {
  m_half_wave = m_sample_rate / 2;
}

float AudioTriangleOscillator::next_value() {

  float v;

  if( m_position < m_half_wave ) {
    
    v = 1.0 - 2.0 * ((float)m_position / (float)m_half_wave);

  } else {
    
    float p = m_half_wave - (m_position - m_half_wave);

    v = 1.0 - 2.0 * (p / (float)m_half_wave);
  }

  m_position += m_pitch;
  if(m_position > m_sample_rate) m_position -= m_sample_rate;

  return v * m_volume;
}

