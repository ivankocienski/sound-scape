#include <iostream>
#include <unistd.h>

#include "audio/audio.hh"

using namespace std; 

int main( int arch, char** argv ) {

  try {

    AudioService audio;
    audio.init();

    { // volume
      AudioPattern ap;

      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.7, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.5, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.2, 523, 50 );

      ap.beep( audio.oscillator(AudioService::WF_SILENCE), 0.0, 0, 50 );

      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.2, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.5, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.7, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 523, 50 );

      ap.beep( audio.oscillator(AudioService::WF_SILENCE), 0.0, 0, 50 );

      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.7, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.5, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.2, 523, 50 );

      ap.beep( audio.oscillator(AudioService::WF_SILENCE), 0.0, 0, 50 );

      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.2, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.5, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.7, 523, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 523, 50 );

      audio.set_pattern( 0, ap );
    }


    // channel, pattern
    audio.play( 0, 0 );

    audio.start();

    sleep(10);

  } catch( const AudioException &x ) {

    cerr << "error: (" << x.function() << ") " << x.message() << endl;
  }

  return 0;

}
