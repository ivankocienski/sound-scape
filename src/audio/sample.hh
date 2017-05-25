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
  Sample(float*, int, int);

  bool load(const char *);

  sample_info_t sample_info() const;
  std::list<Sample> segment(int);

  void soften(int);
  bool is_loud(float) const ;

private:

  std::vector<float> m_samples;
  int m_sample_rate;
};

}
