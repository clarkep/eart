#include "file_q.h"
#include "quiz.h"
#include "synth1.h"
#include "theory.h"
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;
using namespace std::string_literals;

static int BASIC[5][4] = {{S_C, 0, 4, 0},
                         {S_E, 0, 4, 1},
                         {S_F, 0, 4, 0},
                         {S_G, 0, 4, 0},
                         {S_A, 0, 4, 1}};

std::vector<ChordQItem> triads_generator(int arr[][4], int len)
{
    std::vector<ChordQItem> ret;
    for (int i=0; i<len; i++) {
        std:string suf;
        if(arr[i][3]) { suf = "min"s; } 
        else { suf = "maj"s; }
      
        ret.push_back(ChordQItem(triad(Note(SN(arr[i][0], arr[i][1]), arr[i][2]),
                                       arr[i][3]), suf));
    }
    return ret;
}

std::vector<ChordQItem> triads_basic()
{ 
    return triads_generator(BASIC, 5);
}

/*
std::vector<ChordQItem> triads_2()
{
    std::vector<ChordQItem> ret{
            ChordQItem(major_triad(Note(SN(S_C, 0), 4)), "M"s),
            ChordQItem(minor_triad(Note(SN(S_E, 0), 4)), "m"s),
            ChordQItem(major_triad(Note(SN(S_F, 0), 4)), "M"s),
            ChordQItem(major_triad(Note(SN(S_G, 0), 4)), "M"s),
            ChordQItem(minor_triad(Note(SN(S_A, 0), 4)), "m"s) };

}
*/
