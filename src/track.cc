#include "track.hh"
#include <list>
#include <cstdlib>

using namespace std;
using namespace audio;

Track::Track() {
  m_sleep = 0;
  m_sleep_len = 0;
}

void Track::load(const string &path) {
  
  list<Sample> segments;

  Sample sample; 
  sample.load(path.c_str());
  cout << "loading " << path << endl; 

  m_sleep_len = 50 + rand() % 100;

  sample.segment(segments, m_sleep_len); 

  segments.remove_if( [](const Sample &sample) { return !sample.is_loud(0.1); } );

  m_samples.assign(segments.begin(), segments.end());
}

//void Track::prune_quiet() {
//}

bool Track::is_empty() {
  return m_samples.size() == 0;
}

void Track::soften() {
  for(auto it : m_samples)
    it.soften(20);

}

void Track::tick(Service &audio, int delta) {
  
  if(m_sleep > 0) {
    m_sleep -= delta;
    return;
  }

  m_sleep = m_sleep_len;
  
  audio.queue(m_samples[rand() % m_samples.size()]);
}


