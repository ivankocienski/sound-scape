#include "track.hh"
#include "misc.hh"

#include <list>
#include <cstdlib>

using namespace std;
using namespace audio;

Track::Track() {
  m_sleep = 0;
  m_sleep_len = 0;
}

void Track::load(const string &path) {
  cout << "loading " << path << endl; 
  
  list<Sample> segments;

  Sample sample; 
  sample.load(path.c_str());
  sample.normalize();

  m_sleep_len = 100 + rand() % 200;

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
    it.soften(40);

}

void Track::tick(Service &audio, int delta) {
  
  if(m_sleep > 0) {
    m_sleep -= delta;
    return;
  }

  m_sleep = m_sleep_len * (rand() % 10);
  
  audio.queue(m_samples[rand() % m_samples.size()], frand());
  //0.5 + 0.5 * frand());
}


