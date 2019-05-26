#ifndef __INTERFACE_H_INCLUDED__
#define __INTERFACE_H_INCLUDED__

#include "synth1.h"
#include <vector>
#include <string>

struct ChordQItem : Chord {
    ChordQItem(std::vector<Note> nv, Key k, std::string suf) : Chord(nv, k), suffix(suf) {}
    ChordQItem() : ChordQItem({}, Key("C"), "") {}
    ChordQItem transpose(s_note intv) const;
    std::string suffix;
};

struct MultiQItem {
    MultiQItem(std::vector<ChordQItem> c, std::string i) : chords(c), info(i) {}
    MultiQItem() : MultiQItem({}, "") {}
    std::vector<ChordQItem> chords;
    std::string info;
};

class Quiz
{
public:
    Quiz(NoteSynth *s) : synth(s) {}
    virtual void begin() {}
    virtual int next_round() {}
protected:
    NoteSynth* synth;
};

class SingleQuiz : public Quiz
{
public:
    SingleQuiz(NoteSynth *s) : Quiz(s) {}
    virtual void begin() {}
    virtual int next_round();
protected:
    virtual ChordQItem get_item() {}
};

class MultiQuiz : public Quiz
{
public:
    MultiQuiz(NoteSynth *s) : Quiz(s) {}
    virtual void begin() {}
    virtual int next_round();
protected:
    virtual MultiQItem get_item() {}
};

#endif
