#include "theory.h"
#include "synth1.h"
#include "interface.h"
#include "file_q.h"
#include "quiz.h"
#include "list_q.h"
#include <string>
#include <vector>
#include <iostream>
#include <ctime>

using namespace std;
using namespace std::string_literals;

/*
vector<ChordQItem> gen1()
{
    std::string s1("foo");
    vector<Note> cmaj7{Note(C_C, 4), Note(C_E, 4), Note(C_G, 4), Note(C_B, 4)};
    ChordQItem q1(cmaj7, Key("C"s), "maj7"s);
    q1.key=Key("C");
    q1.notevec=cmaj7;
    q1.suffix="maj7";

    std::string s2("Cmin7");
    vector<Note> cmin7{Note(C_C, 4), Note(C_E_FLAT, 4), Note(C_G, 4), Note(C_B_FLAT, 4)};
    ChordQItem q2(cmin7, Key("Cm"), "min7"s); 

    return vector<ChordQItem>{q1, q2};
}
*/

int chord_quiz_basic()
{
	auto seed = 5001;
	srand(seed);
	NoteSynth *synth = new NoteSynth();
	Quiz *quiz = new ListQuiz(synth, &basic_triads);

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
