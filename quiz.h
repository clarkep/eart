/*
 * quiz.h
 * This file contains all the actual quizzes- plus the quiz item object.
 * Author: Paul Clarke
 *
 */

#ifndef __QUIZ_H_INCLUDED__
#define __QUIZ_H_INCLUDED__

#include "theory.h"
#include <vector>
#include <string>

using namespace std::string_literals;

class ChordQItem
{
public:
    ChordQItem(Chord ch, std::string suf) : chord(ch), suffix(suf) {}
    ChordQItem() : ChordQItem(Chord({}, Key("C"s)), ""s) {}
    ChordQItem transpose(s_note intv) const;
    std::vector<Note> get_notevec() { return this->chord.notevec; }
    Key get_key() { return this->chord.key; } 
    std::string suffix;
    Chord chord;
};

class MultiQItem {
public:
    MultiQItem(std::vector<ChordQItem> c, std::string i) : chords(c), info(i) {}
    MultiQItem() : MultiQItem({}, ""s) {}
    std::vector<ChordQItem> chords;
    std::string info;
};

//stackoverflow.com/questions/6942273
template <typename I>
I vec_rand_element(std::vector<I> v)
{
    return v.at(rand() % v.size());
}

std::vector<ChordQItem> transpose(std::vector<ChordQItem> chords, s_note intv);
std::vector<ChordQItem> transpose_r(std::vector<ChordQItem> chords, int lower, int upper,
                                    int min_sharps, int max_sharps);
int max_mn(std::vector<ChordQItem> chords);
int min_mn(std::vector<ChordQItem> chords);



/* transpose q: Tranpose a vector of chord quiz items. */
//std::vector<ChordQItem> transpose_q(std::vector<ChordQItem> chords, s_note intv);

/* transpose_rand: Randomly transpose a chord or chords. Generator for other quizzes.

   Given a chord in the key k, generate a ChordQItem of that chord transposed to
   a random key in the same mode, and put somewhere in the range of midi notes
   [min_mn, max_mn]. The chord will be transposed to a key with a maximum of
   flatsharp_limit flats or sharps(if flatsharp_limit is negative,
   Key::get_flatsharp_limit() will be used). The name of the QItem will be
   generated by a given suffix appended to the key name. If show_lowest is true,
   then the value of the lowest note will be appended to this name. */

/* triad in a random key and inversion */
ChordQItem major_triad_quiz();
/* root position major and minor seventh chords */

MultiQItem maj_root_movements();

#endif
