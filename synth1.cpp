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

void NoteSynth::play_note(Note n)
{
    int midi_n = n.get_midi_n();
    fluid_synth_noteon(synth, 0, midi_n, 127);
    sleep(SUSTAIN_TIME);
    fluid_synth_noteoff(synth, 0, midi_n);
}

void NoteSynth::play_chord(vector<Note> notevec)
{
    int midi_n = 0;
    for (int i = 0; i < notevec.size(); i++) {
        midi_n = notevec[i].get_midi_n();
        fluid_synth_noteon(synth, 0, midi_n, 127);
    }
    sleep(SUSTAIN_TIME);
    for (int i = 0; i < notevec.size(); i++) {
        midi_n = notevec[i].get_midi_n();
        fluid_synth_noteoff(synth, 0, midi_n);
    }
}

/*
int main(int argc, char** argv)
{
    Key::set_modal(true);
    cout << "Flat-sharp limit: " << Key::get_flatsharp_limit() << endl;
    cout << "Modal: " << Key::get_modal() << endl;
    Key k(0, 2);
    fluid_synth_t* synth;

    int ch[] = {60, 64, 67, -1};
    shift_chord(ch, -7);

    synth = setup_fluid_p();
    play_chord(ch, synth);

    teardown_fluid_p();
    return 0;
}

int play_chord(int notes[MAX_CHORD_LENGTH], fluid_synth_t* synth)
{
	int i, n;
	for (i = 0; (n = notes[i])>0; i++) {
		fluid_synth_noteon(synth, 0, notes[i], 80);
	}
	sleep(SUSTAIN_TIME);
	for (i = 0; (n = notes[i])>0; i++) {
		fluid_synth_noteoff(synth, 0, notes[i]);
	}
	return 0;
}

int shift_chord(int notes[MAX_CHORD_LENGTH], int shift)
{
	int i, n;
	for (i=0; (n=notes[i])>0; i++) {
		if (n + shift >= 0) {
			notes[i] = n + shift;
		} else {
			return 1;
		}
	}
	return 0;
}
*/
