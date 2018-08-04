#include <string>
#include "theory.h"
#include "synth1.h"

using namespace std;

int main(int argc, char** argv)
{
	NoteSynth synth = NoteSynth();
	Note n = Note(C_F, 4);
	//synth.play_note(n);

	Note* ch[MAX_CHORD_LENGTH];
	ch[0] = new Note(S_C, 0, 4);
	ch[1] = new Note(C_G, 3);
	ch[3] = new Note(S_A, 1, 4);
	ch[2] = new Note(C_E, 4);

	synth.play_chord(ch, 4);

}
