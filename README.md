# eart - Ear trainer

eart is a simple ear training program based on fluidsynth.

## building
run ```$ make ```. 
The Makefile assumes the soundfont is at "/usr/share/sounds/sf2/FluidR3_GM.sf2". Otherwise, set
$SOUNDFONT before running make. 

## running
```$ eart [level] [-m]```
level is an integer 1-5 which selects the difficulty.
Add -m to test melodies instead of chords. 
