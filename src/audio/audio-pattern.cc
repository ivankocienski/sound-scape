
#include "audio-pattern.hh"
#include "audio-oscillator-base.hh"

using namespace std;

/*
 * AudioPattern::Tone
 *
 * wrap all the nasty business of putting together a tone
 *
 */

AudioPatternTone::AudioPatternTone( const audio_oscillator_ptr o, float v, int p, int ms ) : m_osc(o) {
  m_volume = v;
  m_pitch  = p;
  m_msec   = (float)ms / 1000.0;
}

int AudioPatternTone::sample_len() {
  return m_msec * (float)m_osc->sample_rate();
}

void AudioPatternTone::render( audio_pattern_buffer_t &out, int offset ) {
  
  m_osc->configure( m_pitch, m_volume );

  int len = sample_len();
  int pos = 0;

  int taper_len = 0.002 * (float)m_osc->sample_rate();

  int taper_start = taper_len;
  int taper_stop  = len - taper_len;
  
  float taper_inc = 1.0 / (float)taper_len;
  float taper_vol = 0;

  while(len) {

    if( pos < taper_start ) {
      taper_vol += taper_inc;
      if( taper_vol > 1 ) taper_vol = 1;
    }

    if( pos > taper_stop ) {
      taper_vol -= taper_inc;
      if( taper_vol < 0 ) taper_vol = 0;
    }

    out[ offset + pos ] = m_osc->next_value() * taper_vol;

    len--;
    pos++;
  }
}

/*  
 * AudioPattern
 *
 * capture patterns from upstream, render then to a buffer for later playback
 *
 */

AudioPattern::AudioPattern() {
  m_tones_length = 0;
}

void AudioPattern::beep( const audio_oscillator_ptr osc, float vol, int pitch, int msec ) {

  AudioPatternTone t( osc, vol, pitch, msec );

  m_tones_length += t.sample_len();
  
  m_tones.push_back(t);
}

void AudioPattern::render( audio_pattern_buffer_t & out ) {

  int offset = 0;

  out.resize( m_tones_length );

  for( tone_list_t::iterator it = m_tones.begin(); it != m_tones.end(); it++ ) {

    it->render( out, offset );

    offset += it->sample_len();
  }
}

