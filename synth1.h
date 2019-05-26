#ifndef __SYNTH1_H_INCLUDED__
#define __SYNTH1_H_INCLUDED__

#include "theory.h"
#include <fluidsynth.h>
#include <string>
#include <vector>

#define SUSTAIN_TIME 2
#define MAX_CHORD_LENGTH 10
#define MAX_CLASSNAME_LENGTH 11

class NoteSynth
{
public:
    NoteSynth();
    ~NoteSynth();
    void note_on(Note n);
    void note_off(Note n);
    void chord_on(std::vector<Note> notevec);
    void chord_off(std::vector<Note> notevec);
    //void panic(); TODO
    void play_note(Note n);
    void play_chord(std::vector<Note> notevec);
private:
    fluid_settings_t* settings;
    fluid_audio_driver_t* adriver;
    fluid_synth_t* synth;
    int sfont_id;
};

#endif
