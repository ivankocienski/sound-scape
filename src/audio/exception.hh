#pragma once

#include "common.hh"

namespace audio {

class Exception : public std::exception {
private:

  std::string m_function;
  PaError m_pa_error;

public:

  Exception( const char*, PaError );
  ~Exception() throw() {}

  const std::string& function() const;

  std::string message() const;
};

} // namespace audio
