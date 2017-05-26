
#include <cstring>

#include "service.hh"

#define CATCH_PA_ERROR( func, err ) if( (err) != paNoError ) throw Exception( (func), (err) );

namespace audio {

using namespace std;

const int Service::c_sample_rate  = 44100;
const int Service::c_num_channels = 1;
const int Service::c_data_type    = paFloat32;

int audio_callback( 
    const void* in_b,
    void* out_b, 
    unsigned long len, 
    const PaStreamCallbackTimeInfo* info, 
    const PaStreamCallbackFlags flags, 
    void *data ) {

  Service *audio = (Service*)data;
  float *buffer = (float*)out_b;

  return audio->callback( buffer, len ); 
} 

Service::Service() {
  m_stream = NULL;
  m_volume = 1.0;
  m_paused = false;
}

Service::~Service() {

  PaError err;

  err = Pa_Terminate();
  CATCH_PA_ERROR( "Pa_Terminate", err );
}

int Service::callback( float* buffer, unsigned long buffer_size ) {

  memset( buffer, 0, buffer_size * sizeof(float));

  if( m_paused ) return 0;

  for( auto & it : m_play_tracks ) {

    if(!it.sample_remaining) continue;

    float *dest = buffer;
    float *src = it.sample_ptr;

    int count = buffer_size;
    if(count > it.sample_remaining) count = it.sample_remaining;
    int c = count;

    while(c) {
      *dest += *src * it.volume;

      dest++;
      src++;
      c--;
    }

    it.sample_remaining -= count;
    it.sample_ptr += count;
  }

/*   if(m_sample_ptr) {
 *     int count = buffer_size;
 *     if(count > m_sample_remaining) count = m_sample_remaining;
 * 
 *     // no volume control yet
 *     memcpy(buffer, m_sample_ptr, count * sizeof(float));
 * 
 *     m_sample_remaining -= count;
 *     m_sample_ptr += count;
 * 
 *     if(!m_sample_remaining) {
 *       if(m_sample_queue.size() > 0) {
 *         Sample* sample = m_sample_queue.front();
 *         m_sample_queue.pop_front();
 * 
 *         Sample::sample_info_t sample_info = sample->sample_info();
 * 
 *         m_sample_ptr       = sample_info.data;
 *         m_sample_remaining = sample_info.data_length;
 * 
 *       } else
 *         m_sample_ptr = NULL;
 *     } 
 *   }
 */

  return 0;
}

void Service::init() {

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

  //const PaStreamInfo *stream_info = Pa_GetStreamInfo(m_stream);
  //if( !stream_info ) throw Exception( "Pa_GetStreamInfo", 0 );
  //int sample_rate = stream_info->sampleRate; 
}

void Service::start() { 
  PaError err = Pa_StartStream(m_stream);
  CATCH_PA_ERROR( "Pa_StartStream", err ); 
}

void Service::stop() {
  PaError err = Pa_StopStream(m_stream);
  CATCH_PA_ERROR( "Pa_StopStream", err );
}

bool Service::is_busy() { 
  return false; // FIXME
}

void Service::set_volume( float v ) {
  m_volume = v;
}

float Service::volume() {
  return m_volume;
}

void Service::set_paused(bool p) {
  m_paused = p;
}

bool Service::paused() {
  return m_paused;
}

void Service::queue(Sample &sample, float volume) {

  m_play_tracks.push_back(playback_track_s(sample, volume)); 
}

void Service::sweep_tidy() {

  m_play_tracks.remove_if(
      [](const playback_track_t &track) {
        return track.sample_remaining == 0;
      });
}

} // namespace audio
