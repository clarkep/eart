#include "file_q.h"
#include "quiz.h"
#include "synth1.h"
#include "theory.h"
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


void disp_vec(vector<ChordQItem*> v)
{
    for (int i = 0; i < v.size(); i++) {
        cout << "(" << v.at(i)->key.disp() << ", " << v.at(i)->suffix << "), ";
    }
    cout << endl;
}

std::vector<ChordQItem*> triads_generator(int arr[][4], int len)
{
    std::vector<ChordQItem*> ret;
    for (int i=0; i<len; i++) {
        std:string suf;
        if(arr[i][3]) { suf = "min"s; } 
        else { suf = "maj"s; }

        Chord *ch = triad(Note(SN(arr[i][0], arr[i][1]), arr[i][2]),arr[i][3]);
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
    // disp_vec(basic);
    return basic;
}
