
#include <string.h>

#include "audio-exception.hh"
#include "audio-common.hh"
#include "audio-service.hh"
#include "audio-sine-oscillator.hh"
#include "audio-square-oscillator.hh"
#include "audio-noise-oscillator.hh"
#include "audio-saw-oscillator.hh"
#include "audio-triangle-oscillator.hh"

using namespace std;

const int AudioService::c_sample_rate  = 44100;
const int AudioService::c_num_channels = 1;
const int AudioService::c_data_type    = paFloat32;

int audio_callback( const void* in_b, void* out_b, unsigned long len, const PaStreamCallbackTimeInfo* info, const PaStreamCallbackFlags flags, void *data ) {

  AudioService *audio = (AudioService*)data;
  float *buffer = (float*)out_b;

  return audio->callback( buffer, len ); 
} 

AudioService::AudioService() {
  m_stream = NULL;
  m_volume = 1.0;
  m_paused = false;
}

AudioService::~AudioService() {

  PaError err;

  err = Pa_Terminate();
  CATCH_PA_ERROR( "Pa_Terminate", err );
}

int AudioService::callback( float* buffer, unsigned long buffer_size ) {

  memset( buffer, 0, buffer_size * sizeof(float));

  if( m_paused ) return 0;

  for( vector<AudioChannel>::iterator it = m_channels.begin(); it != m_channels.end(); it++ ) {
    it->render_to( buffer, buffer_size, m_volume );
  }

  return 0;
}

#define MAKE_OSCILLATOR( in, with ) (in).push_back( boost::shared_ptr<AudioOscillatorBase>( (AudioOscillatorBase *)(new with( sample_rate ))));
void AudioService::init() {

  PaError err;

  err = Pa_Initialize();
  CATCH_PA_ERROR( "Pa_Initialize", err );

  err = Pa_OpenDefaultStream(
    &m_stream,
    0,
    c_num_channels,
    c_data_type,
    c_sample_rate,
    paFramesPerBufferUnspecified,
    audio_callback,
    this
  );

  CATCH_PA_ERROR( "Pa_OpenDefaultStream", err ); 

  //m_waveforms.resize(WF_COUNT);

  m_patterns.resize(16);
  m_channels.resize(2);
  
  const PaStreamInfo *stream_info = Pa_GetStreamInfo(m_stream);
  //if( !stream_info ) throw AudioException( "Pa_GetStreamInfo", 0 );
  int sample_rate = stream_info->sampleRate; 
  
  MAKE_OSCILLATOR( m_oscillators, AudioSineOscillator );
  MAKE_OSCILLATOR( m_oscillators, AudioSquareOscillator );
  MAKE_OSCILLATOR( m_oscillators, AudioNoiseOscillator );
  MAKE_OSCILLATOR( m_oscillators, AudioSawOscillator );
  MAKE_OSCILLATOR( m_oscillators, AudioTriangleOscillator );
  MAKE_OSCILLATOR( m_oscillators, AudioOscillatorBase ); // silence
}

void AudioService::start() {

  for( vector<AudioChannel>::iterator it = m_channels.begin(); it != m_channels.end(); it++ )
    it->start();

  PaError err = Pa_StartStream(m_stream);
  CATCH_PA_ERROR( "Pa_StartStream", err ); 
}

void AudioService::stop() {
  PaError err = Pa_StopStream(m_stream);
  CATCH_PA_ERROR( "Pa_StopStream", err );
}

bool AudioService::is_busy() {

  for( vector<AudioChannel>::iterator it = m_channels.begin(); it != m_channels.end(); it++ )
    if( it->is_busy() ) return true;

  return false;
}

void AudioService::play( int ch, int pn ) {
  m_channels[ch].play( m_patterns[pn] );
}

void AudioService::loop( int ch, int pn ) {
  m_channels[ch].loop( m_patterns[pn] );
}

const audio_oscillator_ptr & AudioService::oscillator( int i ) {
  return m_oscillators[i];
}

void AudioService::set_pattern( int id, AudioPattern& ap ) {
  ap.render( m_patterns[id] );
}

void AudioService::set_volume( float v ) {
  m_volume = v;
}

AudioChannel& AudioService::channel(int i) {
  return m_channels[i];
}
