/*
 * quiz.cpp
 * This file contains the quizzes and the quiz item object.
 * Author: Paul Clarke
 *
 */

#include "quiz.h"
#include "theory.h"
#include <vector>
#include <string>
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

// TODO: refactor so that each key has an equal chance of being selected, instead of each note.
ChordQItem transpose_q(std::vector<Note> *chord, int min_mn, int max_mn,
                      string suffix, Key k, int flatsharp_limit, bool show_lowest)
{
    int fs_limit, old_flatsharp_limit;
    if (flatsharp_limit < 0) {
        fs_limit = Key::get_flatsharp_limit();
    } else {
        fs_limit = flatsharp_limit;
        old_flatsharp_limit = Key::get_flatsharp_limit();
        Key::set_flatsharp_limit(max(6, flatsharp_limit));
    }

    Note top = *max_element(chord->begin(), chord->end());
    Note bottom = *min_element(chord->begin(), chord->end());
    int uprange = max_mn - top.get_midi_n();
    int downrange = bottom.get_midi_n() - min_mn;

    vector<int> *keys = randints(-fs_limit, fs_limit+1);
    Key k1 = key_from_sharps(keys->back(), k.get_mode());
    int octave;
    keys->pop_back();
    bool key_works = false;
    while (!key_works) {
        int up_cintv = positive_modulo(k1.get_chrom_n() - k.get_chrom_n(), 12);
        int up_octaves = uprange/12 + ((uprange % 12) > up_cintv ? 1 : 0);
        int down_cintv = 12 - up_cintv;
        int down_octaves = downrange/12 + ((downrange % 12) >= down_cintv ? 1 : 0);

        if (up_octaves || down_octaves)
        {
            key_works = true;
            octave = randint(-down_octaves, up_octaves);
        } else {
            if (keys->size()) {
                k1 = key_from_sharps(keys->back(), k.get_mode());
                keys->pop_back();
            } else {
                throw logic_error("transpose_q: no possibilities with given"
                    " range [min_mn, max) and flatsharp_limit");
            }
        }
    }

    ChordQItem result;
    vector<Note> *new_chord = new vector<Note>();
    for (int i = 0; i < chord->size(); i++) {
        Note n = chord->at(i).ktranspose(k, k1, octave);
        new_chord->push_back(n);
    }
    Note new_low_note = *min_element(new_chord->begin(), new_chord->end());
    string chord_name = k1.disp() + suffix +
      (show_lowest ? ("(low=" + new_low_note.disp() + ")") : "");
    result.notevec = new_chord;
    result.name = chord_name;

    if (flatsharp_limit != -1) {
        Key::set_flatsharp_limit(old_flatsharp_limit);
    }
    return result;
}

ChordQItem quiz_root_pos_majmin_7ths()
{
    vector<Note> cmaj7{Note(C_C, 4), Note(C_E, 4), Note(C_G, 4), Note(C_B, 4)};
    vector<Note> cmin7{Note(C_C, 4), Note(C_E_FLAT, 4), Note(C_G, 4), Note(C_B_FLAT, 4)};
    int maj = (rand() % 2);
    switch(maj)
    {
        case 0: {
            return transpose_q(&cmin7, 35, 85, "min7");
            break;
        }
        case 1: {
            return transpose_q(&cmaj7, 35, 85, "maj7");
            break;
        }
    }
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

    vector<Note> *notes = new vector<Note>();
    for(int i = 0; i < 3; i++)
    {
        Note n = Note(k, oct, SN(2*i, 0));
        if (inv > i) {
            n = n.ctranspose(12);
        }
        notes->push_back(n);
    }
    ChordQItem ret;
    ret.notevec = notes;
    ret.name = "t";
    return ret;
}
