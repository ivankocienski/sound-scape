
#include "sample.hh"
#include <sndfile.h>

namespace audio {

using namespace std;

Sample::Sample() { 
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

  cout << "frames: " << sf_info.frames << endl; 

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

} // namespace audio
