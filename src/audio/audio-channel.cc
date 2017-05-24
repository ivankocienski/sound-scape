
#include "audio-channel.hh"

AudioChannel::AudioChannel() { 
  m_queue_size = 0;
  m_loop = 0;
}

void AudioChannel::render_to( float *buffer, int buflen, float master_volume ) {

  if( !m_queue_size ) return;

  while( buflen ) {

    *buffer += *m_pattern_it;
    m_pattern_it++;

    if( m_pattern_it == m_pattern_queue.front()->end() ) {

      if(m_loop) {
        m_pattern_it = m_pattern_queue.front()->begin();

      } else {

        m_buffer_lock.lock();
        
        m_pattern_queue.pop_front();
        m_queue_size--;
        if(!m_queue_size) {
          m_buffer_lock.unlock();
          return;
        }

        m_pattern_it = m_pattern_queue.front()->begin();

        m_buffer_lock.unlock();
      }
    } 

    buffer++;
    buflen--;
  }
}

bool AudioChannel::is_busy() {
  return m_queue_size > 0;
}

void AudioChannel::start() {
  if( m_queue_size )
    m_pattern_it = m_pattern_queue.front()->begin();
}

void AudioChannel::play( audio_pattern_buffer_t& pb ) { 

  if( m_loop ) return;

  m_pattern_queue.push_back( &pb );

  m_buffer_lock.lock();

  if( !m_queue_size )
    m_pattern_it = m_pattern_queue.front()->begin();

  m_queue_size++;

  m_buffer_lock.unlock();
}

void AudioChannel::loop( audio_pattern_buffer_t& pb ) { 
  play(pb);
  m_loop = true;
}

bool AudioChannel::is_looping() {
  return m_loop;
}

void AudioChannel::stop_looping() {
  m_loop = false;
}
