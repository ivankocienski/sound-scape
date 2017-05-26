# Sound Scape

A simple program that plays a cacophony of sound to blot out your loud neighbours who seem to have no idea that talking loudly in your garden at 3AM really irritates people. No joke.

## Requirements

- C++ 11
- boost
- pkg-config
- cmake
- libsndfile
- portaudio

Built and tested on a gentoo machine.

## to build

    mkdir -p DIR/repo DIR/build

    git clone git@github.com:ivankocienski/sound-scape.git DIR/repo

    cd DIR/build

    cmake ../repo

    make

    ./speech-noise


## Copyright / Licence

The code is released under the BSD licence.

The audio samples are naughty and I do not lay claim to copyright on them. Used without permission.
