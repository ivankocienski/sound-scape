
#include "exception.hh"

using namespace std;

namespace audio {

Exception::Exception( const char* f, PaError e ) : exception() {
  m_function = f;
  m_pa_error = e;
}

const string& Exception::function() const {
  return m_function;
}

string Exception::message() const {
  return string(Pa_GetErrorText(m_pa_error));
}

} // namespace audio
