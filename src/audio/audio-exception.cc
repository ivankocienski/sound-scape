
#include "audio-exception.hh"

using namespace std;

AudioException::AudioException( const char* f, PaError e ) : exception() {
  m_function = f;
  m_pa_error = e;
}

const std::string& AudioException::function() const {
  return m_function;
}

std::string AudioException::message() const {
  return string(Pa_GetErrorText(m_pa_error));
}
