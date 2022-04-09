#include "file_q.h"
#include "quiz.h"
#include "synth1.h"
#include "theory.h"
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;
using namespace std::string_literals;

std::vector<ChordQItem> basic_triads()
{ 
    Key k(rand() % 12, MAJOR);
    
    return {ChordQItem(major_triad(k.scale_note(4, 0)), "M"s),
            ChordQItem(major_triad(k.scale_note(4, 4)), "M"s)};

}
