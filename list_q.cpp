#include "list_q.h"
#include "interface.h"
#include "quiz.h"
#include "synth1.h"
#include "theory.h"
#include "helpers.h"
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace std::string_literals;

static int BASIC[5][4] = {{S_C, 0, 4, 0},
                         {S_E, 0, 4, 1},
                         {S_F, 0, 4, 0},
                         {S_G, 0, 4, 0},
                         {S_A, 0, 4, 1}};

static  int LEVEL2[3][4] = {{S_D, 0, 4, 1},
                            {S_G, 0, 4, 1},
                            {S_B, -1, 4, 0}};


ListQuiz::ListQuiz(NoteSynth *s, std::vector<ChordQItem*> (*gen_list)()) :
    SingleQuiz(s) 
{
    this->gen = gen_list;
}

void ListQuiz::begin()
{
    qitems = (*(this->gen))();
    ChordQItem *base = qitems[0];
    shuffle_qitems();
    cur_index = 0;

    cout << "Key = " << base->key.disp() << endl;
    synth->play_chord(base->notevec);
    cout << "Continue(enter) ";
    string resp;
    getline(cin, resp);
    cout << endl;
}

ChordQItem ListQuiz::get_item()
{
    print_vec_verbose(qitems);
    print_verbose("Cur index: ", cur_index, "\n");
    ChordQItem ret;
    if (cur_index < qitems.size()) {
        ret = *qitems.at(cur_index++);
    } else {
        shuffle_qitems();
        cur_index=0;
        ret = *qitems.at(cur_index++);
    }
    print_vec_verbose(ret.notevec);
    return ret;
}

void ListQuiz::shuffle_qitems()
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(qitems.begin(), qitems.end(), g);
}

ostream& operator<<(ostream& os, const ChordQItem& qitem)
{
    os << qitem.key.disp() << qitem.suffix; 
    return os;
}

std::vector<ChordQItem*> triads_generator(int arr[][4], int len, int max_invert=0)
{
    std::vector<ChordQItem*> ret;
    for (int i=0; i<len; i++) {
        std:string suf;
        if(arr[i][3]) { suf = "min"s; } 
        else { suf = "maj"s; }

        Chord *ch = triad(Note(SN(arr[i][0], arr[i][1]), arr[i][2]),arr[i][3], max_invert ? rand()%max_invert : 0);
        ChordQItem *q = new ChordQItem(ch->notevec, ch->key, suf); 
        ret.push_back(q);
    }
    return ret;
}

std::vector<ChordQItem*> triads_basic()
{ 
    std::vector<ChordQItem*> ret = triads_generator(BASIC, 5);
    transpose_r(ret, 50, 80, -6, 6);
    return ret;
}

std::vector<ChordQItem*> triads_level2()
{
    std::vector<ChordQItem*> basic = triads_generator(BASIC, 5); 
    std::vector<ChordQItem*> level2 = triads_generator(LEVEL2, 3);
    basic.insert(basic.end(), level2.begin(), level2.end()); 
    transpose_r(basic, 50, 80, -5, 6);
    print_vec_verbose(basic);
    return basic;
}

std::vector<ChordQItem*> triads_level3()
{
    std::vector<ChordQItem*> basic = triads_generator(BASIC, 5, 3); 
    std::vector<ChordQItem*> level2 = triads_generator(LEVEL2, 3, 3);
    basic.insert(basic.end(), level2.begin(), level2.end()); 
    transpose_r(basic, 50, 80, -5, 6);
    print_vec_verbose(basic);
    return basic;
}
