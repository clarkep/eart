#include <fluidsynth.h>
#include <stdlib.h>
#include "setup.h"

fluid_settings_t* settings;
fluid_synth_t* synth;
fluid_audio_driver_t* adriver;
int sfont_id;
int i, key;

fluid_synth_t* setup_fluid_p(void)
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
  return synth;
}

void teardown_fluid_p(void)
{
  /* Clean up */
  delete_fluid_audio_driver(adriver);
  delete_fluid_synth(synth);
  delete_fluid_settings(settings);
}
