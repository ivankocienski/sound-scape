#pragma once

#include "common.hh"

namespace audio {

class Sample { 
public:

  typedef struct sample_info {
    float *data;
    int data_length;
  } sample_info_t, *p_sample_info_t;

  Sample();

  bool load(const char *);

  sample_info_t sample_info() const;

private:

  std::vector<float> m_samples;
};

}
