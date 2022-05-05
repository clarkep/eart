# HEART - HEARing Trainer

Heart is a simple ear training program based on fluidsynth.

## Building
run ```$ make ```.   
The Makefile assumes the soundfont is at "/usr/share/sounds/sf2/FluidR3_GM.sf2". Otherwise, export
$SOUNDFONT before running make. 

## Running
```$ heart [level] [-m]```
level is an integer 1-5 which selects the difficulty.
Add -m to test melodies instead of chords. 
