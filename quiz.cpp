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
#include <cctype>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace std;

/* randvec: generate a vector of the integers from start to stop-1, randomly sorted. */
vector<int> *randints(int start, int stop)
{
    vector<int> *ret = new vector<int>();
    for (int i = start; i < stop; i++)
    {
        ret->insert(ret->begin() + rand()%(ret->size() + 1), i);
    }
    return ret;
}

int randint(int min, int max)
{
    return min + rand()%(max - min);
}

int min_mn(vector<ChordQItem> chords)
{
    int bottom = 10000;
    for(int i=0; i<chords.size(); i++) {
        vector<Note> chord = chords.at(i).notevec;
        Note b = *min_element(chord.begin(), chord.end());
        if (b.get_midi_n() < bottom) {
            bottom = b.get_midi_n();
        }
    }
    return bottom;
}

int max_mn(vector<ChordQItem> chords)
{
    int top = -10000;
    for(int i=0; i<chords.size(); i++) {
        vector<Note> chord = chords.at(i).notevec;
        Note t = *max_element(chord.begin(), chord.end());
        if (t.get_midi_n() > top) {
            top = t.get_midi_n();
        }
    }
    return top;
}

int SingleQuiz::next_round()
{
    ChordQItem cur = get_item();
    string resp = "r";
    while (resp == "r") {
        cout << "Playing..." << endl;
        synth->chord_on(cur.notevec);
        cout << "Reveal(enter), or repeat(r): ";
        getline(cin, resp);
    }
    cout << cur.key.disp() << cur.suffix << endl;
    cout << "Continue(enter), or quit(q): ";
    getline(cin, resp);

    synth->chord_off(cur.notevec);
    if (resp == "q") {
        return 0;
    } else {
        return 1;
    }
}

ChordQItem Majmin7Quiz::get_item()
{
    return quiz_root_pos_majmin_7ths();
}

vector<ChordQItem> transpose_q(vector<ChordQItem> chords, int lower, int upper,
    int min_sharps, int max_sharps, bool show_lowest)
{
    int top = max_mn(chords);
    int bottom = min_mn(chords);

    int uprange = upper - top;
    int downrange = lower - bottom;
    vector<s_note> trans_options = transpositions(downrange, uprange, min_sharps, max_sharps);
    s_note trans = trans_options.at(rand()%(trans_options.size()));

    vector<ChordQItem> results;
    for (int i=0; i<chords.size(); i++){
        ChordQItem q_item = chords.at(i);
        ChordQItem res;
        res.key = q_item.key.interval_key(trans, q_item.key.get_mode());
        for (int j = 0; j < q_item.notevec.size(); j++) {
            Note n = q_item.notevec.at(j).ktranspose(q_item.key, trans);
            res.notevec.push_back(n);
        }
        Note new_low_note = *min_element(res.notevec.begin(), res.notevec.end());
        res.suffix = q_item.suffix +
          (show_lowest ? ("(low=" + new_low_note.disp() + ")") : "");
        results.push_back(res);
    }
    return results;
}

ChordQItem quiz_root_pos_majmin_7ths()
{
    vector<Note> cmaj7{Note(C_C, 4), Note(C_E, 4), Note(C_G, 4), Note(C_B, 4)};
    vector<Note> cmin7{Note(C_C, 4), Note(C_E_FLAT, 4), Note(C_G, 4), Note(C_B_FLAT, 4)};
    int maj = 0; // TODO: revert
    ChordQItem q;
    switch(maj)
    {
        case 0: {
            q.key=Key("Cm");
            q.notevec=cmin7;
            q.suffix="min7";
            break;
        }
        case 1: {
            q.key=Key("C");
            q.notevec=cmaj7;
            q.suffix="maj7";
            break;
        }
    }
    return transpose_q({q}, 35, 85, -6, 6).at(0);
}

vector<ChordQItem> maj_root_movements()
{
    vector<Note> cmaj{Note(C_C, 4), Note(C_E, 4), Note(C_G, 4)};
    ChordQItem orig = {cmaj, Key("C"), ""};
    ChordQItem res1 = transpose_q({orig}, 40, 80, -6, 6).at(0);
    ChordQItem res2 = transpose_q({res1}, min_mn({res1})-11, max_mn({res1})+11, -6, 6).at(0);
}

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
    ChordQItem ret;
    ret.notevec = notes;
    ret.suffix = "t";
    return ret;
}
