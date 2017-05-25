
#include "sample.hh"
#include <sndfile.h>

namespace audio {

using namespace std;

Sample::Sample() { 
  m_sample_rate = 0;
}

Sample::Sample(float *src_ptr, int src_len, int sample_rate) {
  m_sample_rate = sample_rate;

  m_samples.resize(src_len);

  memcpy(m_samples.data(), src_ptr, src_len * sizeof(float));
}

bool Sample::load(const char *path) {
  SNDFILE	*sf_file;
  SF_INFO	 sf_info;

  cout << "loading " << path << endl;

  memset(&sf_info, 0, sizeof(sf_info));

  sf_file = sf_open (path, SFM_READ, &sf_info);
	if(!sf_file) {
    cerr << "failed to open file: " << sf_strerror(NULL) << endl;
    return false; 
  }

  if(sf_info.channels != 1) {
    cerr << "error: audio source not single channel" << endl;
    sf_close(sf_file);
    return false;
  }

  cout << "frames      : " << sf_info.frames << endl; 
  cout << "sample-rate : " << sf_info.samplerate << endl;

  m_sample_rate = sf_info.samplerate;

  size_t buffer_size = sf_info.frames;
  m_samples.resize(buffer_size);
  float *buffer_ptr = m_samples.data();

  while(buffer_size > 0) {
    int read_count = sf_read_float(sf_file, buffer_ptr, buffer_size);
    if(read_count == -1) {
      cerr << "something went wrong reading audio file" << endl;
      break;
    }

    buffer_size -= read_count;
    buffer_ptr  += read_count;
  }

  sf_close(sf_file);

  return true;
}

Sample::sample_info_t Sample::sample_info() const {
  sample_info_t si;

  si.data = (float *)m_samples.data();
  si.data_length = m_samples.size();

  return si;
}

list<Sample> Sample::segment(int segment_len_ms) {

  list<Sample> segments;
  int segment_len = (m_sample_rate / 1000.0) * segment_len_ms;

  float *buffer_ptr = m_samples.data();
  int buffer_count = m_samples.size();

  while(buffer_count >= segment_len) {

    segments.push_back(Sample(buffer_ptr, segment_len, m_sample_rate));

    buffer_ptr   += segment_len;
    buffer_count -= segment_len;
  } 

  return segments;
}

void Sample::soften(int margin_len_ms) {
  
  int margin_len_samples = (m_sample_rate / 1000.0) * margin_len_ms;

  if(margin_len_samples > m_samples.size() / 2)
    return;

  float amp = 0;
  float step = 1.0 / margin_len_samples;
  int front = 0;
  int back = m_samples.size() - 1;

  while(margin_len_samples) {
    m_samples[front] *= amp;
    m_samples[back]  *= amp;

    amp += step;
    front++;
    back--;
    margin_len_samples--;
  }
}

bool Sample::is_loud(float threshold) const {

  for(auto it : m_samples) {
    float v = it;
    if(v < 0) v = -v;

    if(v >= threshold) return true;
  }

  return false;
}

} // namespace audio
