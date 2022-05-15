/*
 * quiz.cpp
 * This file contains the quizzes and the quiz item object.
 * Author: Paul Clarke
 *
 */

#include "quiz.h"
#include "theory.h"
#include "synth1.h"
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace std;


void transpose_r(vector<ChordQItem*> chords, int lower, int upper,
                               int min_sharps, int max_sharps)
{
    // cout << "Chord range" << min_mn(chords) << " " << max_mn(chords) << endl;
    int uprange = upper - max_mn(chords);
    int downrange = lower - min_mn(chords);
    // cout << "Range: " << uprange << " " << downrange << endl;
    vector<s_note> trans_options = transpositions(downrange, uprange, min_sharps, max_sharps);
    s_note trans = trans_options.at(rand()%(trans_options.size()));
    transpose(chords, trans);
}

void transpose(vector<ChordQItem*> chords, s_note intv)
{
    for (int i=0; i<chords.size(); i++) {
        chords.at(i)->transpose(intv);
    }
}

int min_mn(vector<ChordQItem*> chords)
{
    int bottom = 10000;
    for(int i=0; i<chords.size(); i++) {
        Note b = chords.at(i)->get_min();
        if (b.get_midi_n() < bottom) {
            bottom = b.get_midi_n();
        }
    }
    return bottom;
}

int max_mn(vector<ChordQItem*> chords)
{
    int top = -10000;
    for(int i=0; i<chords.size(); i++) {
        Note t = chords.at(i)->get_max();
        if (t.get_midi_n() > top) {
            top = t.get_midi_n();
        }
    }
    return top;
}

/*
MultiQItem maj_root_movements()
{
    vector<Note> cmaj{Note(C_C, 4), Note(C_E, 4), Note(C_G, 4)};
    ChordQItem orig = ChordQItem(cmaj, Key("C"), "");
    ChordQItem *res1 = &transpose_r({orig}, 40, 80, -6, 6).at(0);
    s_note intv = vec_rand_element(transpositions(-11, 11, -6, 6));
    res1->transpose(intv);
    return MultiQItem({res1, res2}, sintv_str(intv));
}
*/

ChordQItem major_triad_quiz()
{
    //auto seed = time(NULL);
    //cout << seed << endl;
    //srand(seed);

    int inv = rand() % 3;
    int up_limit;
    switch (inv) {
        case 0: up_limit = 84;
            break;
        case 1: up_limit = 79;
            break;
        case 2: up_limit = 75;
            break;
    }
    Note root = Note(28 + (rand() % (up_limit - 28 + 1)));
    Key k = root.to_key(MAJOR);
    int oct =  root.get_octave();

    vector<Note> notes;
    for(int i = 0; i < 3; i++)
    {
        Note n = Note(k, oct, SN(2*i, 0));
        if (inv > i) {
            n = n.ctranspose(12);
        }
        notes.push_back(n);
    }
    ChordQItem ret(notes, Key("C"), "t");
    return ret;
}
