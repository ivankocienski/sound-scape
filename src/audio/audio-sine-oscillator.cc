
#include <math.h>

#include "audio-sine-oscillator.hh"

AudioSineOscillator::AudioSineOscillator(int sr) : AudioOscillatorBase(sr) {

  m_wave.resize(m_sample_rate);

  float a_inc = (M_PI * 2.0) / (float)m_sample_rate;
  float ang   = 0;

  for( int i = 0; i < m_sample_rate; i++ ) {
    m_wave[i] = sin(ang); 
    ang += a_inc;
  }
}

float AudioSineOscillator::next_value() {

  float v = m_wave[m_position] * m_volume;

  m_position += m_pitch;
  if(m_position > m_sample_rate) m_position -= m_sample_rate;

  return v;
}

