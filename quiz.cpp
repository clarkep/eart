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

bool is_numeric(string s)
{
    for (int i=0; i<s.size(); i++) {
        if (!isdigit(s.at(i))) {
            return false;
        }
    }
    return true;
}
ChordQItem ChordQItem::transpose(s_note intv) const
{
    Chord c = Chord::transpose(intv);
    return ChordQItem(c.notevec, c.key, suffix);
}

vector<ChordQItem> transpose_r(vector<ChordQItem> chords, int lower, int upper,
                               int min_sharps, int max_sharps)
{
    int uprange = upper - max_mn(chords);
    int downrange = lower - min_mn(chords);
    vector<s_note> trans_options = transpositions(downrange, uprange, min_sharps, max_sharps);
    s_note trans = trans_options.at(rand()%(trans_options.size()));
    return transpose(chords, trans);
}

vector<ChordQItem> transpose(vector<ChordQItem> chords, s_note intv)
{
    vector<ChordQItem> results;
    for (int i=0; i<chords.size(); i++) {
        results.push_back(chords.at(i).transpose(intv));
    }
    return results;
}

int min_mn(vector<ChordQItem> chords)
{
    int bottom = 10000;
    for(int i=0; i<chords.size(); i++) {
        Note b = chords.at(i).get_min();
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
        Note t = chords.at(i).get_max();
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

int MultiQuiz::next_round()
{
    MultiQItem cur = get_item();
    string resp = "r";
    while (resp != "") {
        if (resp == "r") {
            cout << "Playing..." << endl;
            for(int i = 0; i<cur.chords.size(); i++) {
                synth->play_chord(cur.chords.at(i).notevec);
            }
        } else if (is_numeric(resp)) {
            int pos = stoi(resp) - 1;
            if ((pos>0) && (pos<cur.chords.size())) {
                cout << "Playing..." << endl;
                synth->play_chord(cur.chords.at(stoi(resp)).notevec);
            } else {
                cout << "Out of range." << endl;
            }
        } else {
            cout << "Unknown input." << endl;
        }
        cout << "Reveal(enter), or repeat(r or #): ";
        getline(cin, resp);
    }
    cout << cur.info << endl;
    for (int i=0; i<cur.chords.size(); i++) {
        cout << cur.chords.at(i).key.disp() << cur.chords.at(i).suffix << endl;
    }
    cout << "Continue(enter), or quit(q): ";
    getline(cin, resp);

    if (resp == "q") ? 0 : 1;
}

ChordQItem Majmin7Quiz::get_item()
{
    return quiz_root_pos_majmin_7ths();
}

MultiQItem RootMajMQuiz::get_item()
{
    return maj_root_movements();
}

ChordQItem quiz_root_pos_majmin_7ths()
{
    vector<Note> cmaj7{Note(C_C, 4), Note(C_E, 4), Note(C_G, 4), Note(C_B, 4)};
    vector<Note> cmin7{Note(C_C, 4), Note(C_E_FLAT, 4), Note(C_G, 4), Note(C_B_FLAT, 4)};
    int maj = (rand() % 2);
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
    return transpose_r({q}, 35, 85, -6, 6).at(0);
}

MultiQItem maj_root_movements()
{
    vector<Note> cmaj{Note(C_C, 4), Note(C_E, 4), Note(C_G, 4)};
    ChordQItem orig = ChordQItem(cmaj, Key("C"), "");
    ChordQItem res1 = transpose_r({orig}, 40, 80, -6, 6).at(0);
    ChordQItem res2 = transpose_r({res1}, min_mn({res1})-11, max_mn({res1})+11, -6, 6).at(0);
    return MultiQItem({res1, res2}, "---");
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
    ChordQItem ret(notes, Key("C"), "t");
    return ret;
}
