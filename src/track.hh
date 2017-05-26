#pragma once

#include <string>
#include <vector>

#include "audio/audio.hh"

class Track {
public:

  Track();
  void load(const std::string&);
  //void prune_quiet();
  bool is_empty();
  void soften();

  void tick(audio::Service&, int);
  
private:

  std::vector<audio::Sample> m_samples;
  int m_sleep;
  int m_sleep_len;
};

