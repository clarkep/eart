#include "theory.h"
#include "synth1.h"
#include "interface.h"
#include "file_q.h"
#include "quiz.h"
#include "list_q.h"
#include "helpers.h"
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <ctime>
#include <unistd.h>

#define DEFAULT_LEVEL 1
#define MAX_LEVEL '3'

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

static std::vector<ChordQItem*> (*list_q_levels[])() = {&triads_basic, 
    &triads_level2, &triads_level3};

int process_level_arg(char *arg)
{
    if (strlen(arg)==1 && arg[0]>='1' && arg[0]<=MAX_LEVEL) {
        return arg[0] - '0'; 
    } else {
        fprintf(stderr, "Invalid level argument\n");
        exit(1);
    }
}

int chord_quiz_basic(int level)
{
    time_t seed = time(NULL);
    print_verbose(seed, "\n");
    srand(seed);
    NoteSynth *synth = new NoteSynth();
    Quiz *quiz = new ListQuiz(synth, list_q_levels[level-1]);

    string resp;
    cout << "Begin Quiz? [Y/n]: ";
    getline(cin, resp);
    int cont = (resp == "n") ? 0 : 1;
    if (cont)
        quiz->begin();
    while (cont)
        cont = quiz->next_round();
}

bool verbose_flag=false;

int main(int argc, char** argv)
{
    char c;
    int level = DEFAULT_LEVEL;
    while ((c = getopt (argc, argv, "vl:")) != -1)
    {
        switch (c)
        {
        case 'v':
            verbose_flag = true;
            break;
        case 'l':
            level = process_level_arg(optarg);
            break;
        case '?':
            cout << "Unknown option.\n";
            return 1;
        }
    }

    print_verbose("Running in verbose mode.\n");
    chord_quiz_basic(level);
    return 0;
    //synth.play_note(Note(C_C, 4));
}
