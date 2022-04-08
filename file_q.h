#ifndef __FILE_Q_H_INCLUDED__
#define __FILE_Q_H_INCLUDED__

#include "interface.h"
#include "synth1.h"
#include "quiz.h"
#include <string>
#include <vector>
#include <array>
#include <fstream>

struct MultiQRange : MultiQItem
{
    MultiQRange(std::vector<ChordQItem> c, std::string i, int min, int max)
               : MultiQItem(c, i),  min_mn(min), max_mn(max) {}
    MultiQRange() : MultiQRange({}, "", 0, 127) {}
    int min_mn;
    int max_mn;
};

/* Types of lines in quiz file for the FileMQuiz */
enum LineType {t_blank, t_chord, t_key, t_range, t_invalid};
LineType classify_line(std::string s);
std::vector<Note> get_chord(std::string line);
/*
class FileMQuiz : public MultiQuiz
{
public:
    FileMQuiz(NoteSynth *s, std::string fn);
    void begin() {}
protected:
    MultiQItem get_item();
private:
    std::vector<MultiQRange> items;
};
*/

#endif
