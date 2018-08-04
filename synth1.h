#ifndef __SYNTH1_H_INCLUDED__
#define __SYNTH1_H_INCLUDED__

#include <fluidsynth.h>
#include <string>
#include "theory.h"

#define SUSTAIN_TIME 5
#define MAX_CHORD_LENGTH 10
#define MAX_CLASSNAME_LENGTH 11

class NoteSynth
{
public:
    NoteSynth();
    ~NoteSynth();
    void play_note(Note n);
    void play_chord(Note* notes[MAX_CHORD_LENGTH], int len);
private:
    fluid_settings_t* settings;
    fluid_audio_driver_t* adriver;
    fluid_synth_t* synth;
    int sfont_id;
};

#endif
