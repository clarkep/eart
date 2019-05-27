#ifndef __INTERFACE_H_INCLUDED__
#define __INTERFACE_H_INCLUDED__

#include "quiz.h"
#include "synth1.h"
#include <vector>
#include <string>
#include <fstream>

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

class Majmin7Quiz : public SingleQuiz
{
public:
    Majmin7Quiz(NoteSynth *s) : SingleQuiz(s) {}
    void begin() {}
protected:
    ChordQItem get_item();
};

class RootMajMQuiz : public MultiQuiz
{
public:
    RootMajMQuiz(NoteSynth *s) : MultiQuiz(s) {}
    void begin() {}
protected:
    MultiQItem get_item();
};

class FileMQuiz : public MultiQuiz
{
public:
    FileMQuiz(NoteSynth *s, std::string fn);
    ~FileMQuiz();
    void begin();
protected:
    MultiQItem get_item();
private:
    std::ifstream file;
    std::vector<MultiQItem> items;
};

#endif
