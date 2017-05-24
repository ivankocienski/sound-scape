
#pragma once

#include <vector>
#include <portaudio.h>

#define CATCH_PA_ERROR( func, err ) if( (err) != paNoError ) throw AudioException( (func), (err) );

typedef std::vector<float> audio_waveform_t;
typedef std::vector<float> audio_pattern_buffer_t;
