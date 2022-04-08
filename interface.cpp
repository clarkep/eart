#include "interface.h"
#include "synth1.h"
#include "quiz.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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

bool is_staff_letter(char c) {
    string chars = "ABCDEFGabcdefg";
    return (chars.find(c)!=-1);
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

    return (resp == "q") ? 0 : 1;
}

ListQuiz::ListQuiz(NoteSynth *s, std::vector<ChordQItem> (*gen_list)()) :
    SingleQuiz(s) 
{
    this->gen = gen_list;
}

void ListQuiz::begin()
{
    qitems = (*(this->gen))();
}

ChordQItem ListQuiz::get_item()
{
   int i = rand() % this->qitems.size();
   return this->qitems.at(i);
}

ChordQItem Majmin7Quiz::get_item()
{
    return quiz_root_pos_majmin_7ths();
}

MultiQItem RootMajMQuiz::get_item()
{
    return maj_root_movements();
}
