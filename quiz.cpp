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

// TODO: refactor so that each key has an equal chance of being selected, instead of each note.
ChordQItem transpose_q(std::vector<Note> chord, int min_mn, int max_mn,
                      string suffix, int flatsharp_limit, bool show_lowest)
{
    //auto seed = );
    //srand(seed);

    /* randomly choose what note we're going to end up on */

    Note top = *max_element(chord.begin(), chord.end());
    Note bottom = *min_element(chord.begin(), chord.end());
    if ((top.get_midi_n() > max_mn) || (bottom.get_midi_n() < min_mn)) {
        throw logic_error("transpose_q: input chord must be inside range.");
    }
    int uprange = max_mn - top.get_midi_n();
    int downrange = bottom.get_midi_n() - min_mn;
    int shift = (rand() % (uprange + downrange + 1)) - downrange;

    /* figure out what keys we could be in and choose one*/
    Key k0 = chord[0].get_key();
    int fps = 0;
    int s_note = resolve_chromatic(positive_modulo(k0.get_chrom_n() + shift, 12), k0.get_mode(), fps);
    int sharp_enharms_available = (flatsharp_limit + s_note_flats(s_note, fps) + mode_flats(k0.get_mode())) / 12;
    int flat_enharms_available = (flatsharp_limit - s_note_flats(s_note, fps) - mode_flats(k0.get_mode())) / 12;
    int enharm = 0;
    if (sharp_enharms_available + flat_enharms_available > 0) {
        enharm = (rand() % (sharp_enharms_available + flat_enharms_available + 1)) - flat_enharms_available;
    }
    if (enharm > 0) {
        for (int i = 0; i < enharm; i++) {
            s_note = positive_modulo(s_note - 1, 7);
            if (s_note == S_B || s_note == S_E) {
                fps += 1;
            } else {
                fps += 2;
            }
        }
    } else if (enharm < 0) {
        for (int i = 0; i < abs(enharm); i++) {
            s_note = positive_modulo(s_note + 1, 7);
            if (s_note == S_F || s_note == S_C) {
                fps -= 1;
            } else {
                fps -= 2;
            }
        }
    }
    Key k1 = Key(s_note, fps, k0.get_mode());

    ChordQItem result;
    vector<Note> new_chord;
    int octave_change = (shift == 0) ? 0 : (shift / 12 + (shift < 0 ? -1 : 1));
    for (int i = 0; i < chord.size(); i++) {
        Note n = chord[i].ktranspose(k1, octave_change);
        new_chord.push_back(n);
    }
    Note new_low_note = *min_element(new_chord.begin(), new_chord.end());
    string chord_name = k1.disp() + suffix +
      (show_lowest ? ("(low=" + new_low_note.disp() + ")") : "");
    result.notevec = new_chord;
    result.name = chord_name;
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
            return transpose_q(cmin7, 35, 85, "min7");
            break;
        }
        case 1: {
            return transpose_q(cmaj7, 35, 85, "maj7");
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

    vector<Note> notes;
    for(int i = 0; i < 3; i++)
    {
        Note n = Note(k, oct, 2*i, 0);
        if (inv > i) {
            n = n.ctranspose(12);
        }
        notes.push_back(n);
    }
    ChordQItem ret;
    ret.notevec = notes;
    ret.name = "t";
    return ret;
}
