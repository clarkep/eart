#include "file_q.h"
#include "interface.h"
#include "quiz.h"
#include "theory.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

/* classify_line: classify lines from quiz files. Only guaranteed to be correct
    for valid lines. Invalid lines could be classified as invalid or something
    else. */
LineType classify_line(string s) {
    if (s.size() == 0) {
        return t_blank;
    } else if (is_staff_letter(s.at(0))) {
        for (int i=1; i<s.size(); i++) {
            if (isdigit(s.at(i))) {
                return t_chord;
            } else if (s.at(i)!='#' && s.at(i)!='b') {
                return t_invalid;
            }
        }
        return t_key;
    } else if (isdigit(s.at(0))) {
        return t_range;
    } else {
        return t_invalid;
    }
}

vector<Note> get_chord(string line)
{
    istringstream line_stream(line);
    string word;
    vector<Note> ret;
    while (line_stream >> word) {
        ret.push_back(Note(word));
    }
    return ret;
}
/*
FileMQuiz::FileMQuiz(NoteSynth *s, std::string filename) :  MultiQuiz(s)
{
    ifstream ifs(filename);
    string line;
    bool file_open = ifs.is_open();
    if (file_open) {
        LineType prev_type = t_blank;
        MultiQRange cur_item;
        ChordQItem cur_chord;
        while (getline(ifs, line)) {
            LineType cur_type = classify_line(line);
            if (cur_type == t_invalid) {
                throw logic_error("invalid line in input file.");
            } else if (!((cur_type == t_key) == (prev_type == t_chord))) {
                throw logic_error("invalid line in input file: key must come after each chord.");
            } else if (!((cur_type == t_blank) == (prev_type == t_range))) {
                throw logic_error("invalid line in input file: a blank line must come after note range.");
            } else if (cur_type == t_chord) {
                cur_chord.get_notevec() = get_chord(line);
            } else if (cur_type == t_key) {
                cur_chord.get_key = Key(line);
                cur_item.chords.push_back(cur_chord);
            } else if (cur_type == t_range) {
                istringstream line_stream(line);
                line_stream >> cur_item.min_mn;
                line_stream >> cur_item.max_mn;
                items.push_back(cur_item);
                cur_item = MultiQRange();
            }
            prev_type = cur_type;
        }
    } else {
        throw runtime_error("error opening file");
    }
}


MultiQItem FileMQuiz::get_item()
{
    if (items.size() == 0) {
        throw logic_error("input file contains no test cases");
    }
    MultiQRange item = vec_rand_element(items);
    return MultiQItem(transpose_r(item.chords, item.min_mn, item.max_mn, -6, 6), "");
}
*/
