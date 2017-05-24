
#pragma once

#include <list>
//#include <mutex>

#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>

#include "audio-common.hh"

class AudioChannel {
private:

//  std::mutex m_lock;
  
  std::list<audio_pattern_buffer_t*> m_pattern_queue;

  audio_pattern_buffer_t::iterator m_pattern_it;

  boost::mutex m_buffer_lock;
  
  int m_queue_size;
  bool m_loop;

public:

  AudioChannel();

  AudioChannel( const AudioChannel& ) {
    m_queue_size = 0;
    m_loop = 0;
  }

  AudioChannel& operator=( const AudioChannel& ) { 
    m_queue_size = 0;
    m_loop = 0;
    return *this; 
  }

  bool is_busy();
  void start();

  void play( audio_pattern_buffer_t& );
  void loop( audio_pattern_buffer_t& );
  
  bool is_looping();
  void stop_looping();

  void render_to( float*, int, float );
};

