#include "theory.h"
#include "synth1.h"
#include "quiz.h"
#include <string>
#include <vector>
#include <iostream>
#include <ctime>


using namespace std;

void chord_quiz_basic()
{
	auto seed = time(NULL);
	srand(seed);
	NoteSynth synth = NoteSynth();
	ChordQItem current;
	cout << "Begin Quiz? [Y/n]: ";
	string resp;
	getline(cin, resp);
	while (resp != "q")
	{
		if (resp != "r") {
			current = quiz_root_pos_majmin_7ths();
		}
		synth.chord_on(current.notevec);
		cout << "Press Enter to continue, r to repeat, q to quit: ";
		getline(cin, resp);
		if (resp != "r") {
			cout << current.name << endl;
		}
		synth.chord_off(current.notevec);
	}
}

int main(int argc, char** argv)
{
	chord_quiz_basic();
	//synth.play_note(Note(C_C, 4));
}
