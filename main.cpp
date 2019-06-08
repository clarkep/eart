#include "synth1.h"
#include "interface.h"
#include "file_q.h"
#include <string>
#include <vector>
#include <iostream>
#include <ctime>

using namespace std;

void chord_quiz_basic()
{
	auto seed = 5001;
	srand(seed);
	NoteSynth *synth = new NoteSynth();
	Quiz *quiz = new FileMQuiz(synth, "movements0.txt");

	string resp;
	cout << "Begin Quiz? [Y/n]: ";
	getline(cin, resp);
	int cont = (resp == "n") ? 0 : 1;
	quiz->begin();
	while (cont)
	{
		cont = quiz->next_round();
	}
}

int main(int argc, char** argv)
{
	chord_quiz_basic();
	//synth.play_note(Note(C_C, 4));
}
