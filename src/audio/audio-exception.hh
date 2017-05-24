
#pragma once

#include <string>
#include <exception>

#include <portaudio.h>

class AudioException : public std::exception {
private:

  std::string m_function;
  PaError m_pa_error;

public:

  AudioException( const char*, PaError );
  ~AudioException() throw() {}

  const std::string& function() const;

  std::string message() const;
};
