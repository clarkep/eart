#ifndef __LIST_Q_H_INCLUDED__
#define __LIST_Q_H_INCLUDED__

#include <iostream>
#include <list>

#include "theory.h"
#include "quiz.h"
#include "interface.h"

class ListQuiz: public SingleQuiz
{
public:
    ListQuiz(NoteSynth *s, std::vector<ChordQItem*> (*gen_list)());
    void begin();
protected:
    ChordQItem get_item();
private:
    void shuffle_qitems();
    std::vector<ChordQItem*> (*gen)();
    std::vector<ChordQItem*> qitems;
    int cur_index;
};

ostream& operator<<(ostream& os, const ChordQItem& qitem);

std::vector<ChordQItem*> triads_basic();
std::vector<ChordQItem*> triads_level2();
std::vector<ChordQItem*> triads_level3();

#endif
