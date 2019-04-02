#include "synth1.h"
#include <fluidsynth.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>

using namespace std;

NoteSynth::NoteSynth()
{
  /* Create the settings. */
  settings = new_fluid_settings();
  /* Change the settings if necessary*/
  /* Create the synthesizer. */
  synth = new_fluid_synth(settings);
  /* Create the audio driver. The synthesizer starts playing as soon
     as the driver is created. */
  fluid_settings_setstr(settings, "audio.driver", "pulseaudio");
  adriver = new_fluid_audio_driver(settings, synth);
  /* Load a SoundFont and reset presets (so that new instruments
   * get used from the SoundFont) */
  sfont_id = fluid_synth_sfload(synth, "/usr/share/sounds/sf2/FluidR3_GM.sf2", 1);
}

NoteSynth::~NoteSynth()
{
  /* Clean up */
  delete_fluid_audio_driver(adriver);
  delete_fluid_synth(synth);
  delete_fluid_settings(settings);
}

void NoteSynth::note_on(Note n)
{
    fluid_synth_noteon(synth, 0, n.get_midi_n(), 127);
}

void NoteSynth::note_off(Note n)
{
    fluid_synth_noteoff(synth, 0, n.get_midi_n());
}

void NoteSynth::chord_on(vector<Note> *notevec)
{
    for (int i = 0; i < notevec->size(); i++) {
        note_on(notevec->at(i));
    }
}

void NoteSynth::chord_off(vector<Note> *notevec)
{
    for (int i = 0; i < notevec->size(); i++) {
        note_off(notevec->at(i));
    }
}

void NoteSynth::play_note(Note n)
{
    int midi_n = n.get_midi_n();
    fluid_synth_noteon(synth, 0, midi_n, 127);
    sleep(SUSTAIN_TIME);
    fluid_synth_noteoff(synth, 0, midi_n);
}

void NoteSynth::play_chord(vector<Note> *notevec)
{
    chord_on(notevec);
    sleep(SUSTAIN_TIME);
    chord_off(notevec);
}
