/*
 *  theory.cpp
 *  Classes to represent basic musical objects in equal-tempered harmony-
 *  keys and notes.
 *  Author: Paul Clarke
 *  5/2/18
 *
 *
 * NOTE: Throughout this code, intervals are expressed in a way that will be
 * unusual to most musicians. They are based on the notes of the major scale,
 * as usual, but instead of starting from 1 as the tonic, the tonic is
 * represented by 0. This better reflects the additive nature of melody, and
 * simplifies the calculations somewhat. So the interval usually called a
 * "minor third" would be specified by the numbers 2 and -1, representing 2
 * steps up the major scale and one chromatic shift downward.
 * To stay consistent, the comments use a similar terminology. What is usually
 * called a "major second" is here called a major first. A "perfect fifth" is
 * called a perfect fourth, and so on. An octave becomes a septave. This comment
 * is the only place you'll see the normal terminology. This new terminology is
 * what should have been used from the start in music, though, so I hope you get
 * used to it :).
 */

#include "theory.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <cmath>

using namespace std;

bool s_note_eq(s_note a, s_note b)
{
    return (a.n == b.n) && (a.fps == b.fps);
}

/* https://stackoverflow.com/questions/14997165 */
int positive_modulo(int i, int n) {
    int tmp = i % n;
    return tmp ? i >= 0 ? tmp : tmp + n : 0;
}

int floor_divide(int x, int y) {
    return x >= 0 ? x / y : (x/y) - 1;
}

string s_note_str(int s)
{
    s = s % 7;
    string r = "";
    switch (s) {
        case S_C: r = "C";
            break;
        case S_D: r = "D";
            break;
        case S_E: r = "E";
            break;
        case S_F: r = "F";
            break;
        case S_G: r = "G";
            break;
        case S_A: r = "A";
            break;
        case S_B: r = "B";
            break;
    }
    return r;
}

string s_fps_str(s_note sn) {
    string ret = s_note_str(sn.n);
    for (int i=0; i < abs(sn.fps); i++) {
        ret.append((sn.fps > 0) ? "#" : "b");
    }
    return ret;
}

string c_note_str(c_note c) {
    c = c % 12;
    string r = "";
    switch (c) {
        case C_C: r = "C";
            break;
        case C_C_SHARP: r = "C#/Db";
            break;
        case C_D: r = "D";
            break;
        case C_D_SHARP: r = "D#/Eb";
            break;
        case C_E: r = "E";
            break;
        case C_F: r = "F";
            break;
        case C_F_SHARP: r = "F#/Gb";
            break;
        case C_G: r = "G";
            break;
        case C_G_SHARP: r = "G#/Ab";
            break;
        case C_A: r = "A";
            break;
        case C_A_SHARP: r= "A#/Bb";
            break;
        case C_B: r = "B";
            break;
    }
    return r;
}

string mode_str(mode_i m) {
    if (m == MINOR) {
        return "Minor";
    }
    m = positive_modulo(m, 7);
    string r = "";
    switch (m) {
        case IONIAN: r = "Ionian";
            break;
        case DORIAN: r = "Dorian";
            break;
        case PHRYGIAN: r = "Phrygian";
            break;
        case LYDIAN: r = "Lydian";
            break;
        case MIXOLYDIAN: r = "Mixolydian";
            break;
        case AEOLIAN: r = "Aeolian";
            break;
        case LOCRIAN: r = "Locrian";
            break;
    }
    return r;
}

int thirds_from_six(int sn)
{
    int s2 = positive_modulo(sn, 7);
    int octave = (sn < 6) ? 1 : 0;
    while (((s2 - 6 + octave*7) % 3) != 0) {
        octave += 1;
    }
    return (s2 - 6 + octave*7) / 3;
}

s_note add_intv(s_note base, s_note intv)
{
    s_note res;
    int key_sharps = -s_note_flats(base);
    int nec = thirds_from_six(positive_modulo(intv.n, 7)); //fourths from a major 7th, the first note to get a sharp.
    res.n = positive_modulo(base.n + intv.n, 7);
    res.fps = floor((float) key_sharps / (float) 7) + ((positive_modulo(key_sharps, 7)>=(nec + 1)) ? 1 : 0) + intv.fps;
    return res;
}

//int get_intv(int low_sn, int low_fps, int up_sn, int up_fps, int &intv_fps)
s_note get_intv(s_note lower, s_note upper)
{
    s_note intv;
    c_note chromatic_distance =
    positive_modulo(s_to_c((s_note){upper.n, 0}) - s_to_c((s_note){lower.n, 0}), 12) - lower.fps + upper.fps;
    intv.n = positive_modulo(upper.n - lower.n, 7);
    c_note intv_sn_dist = s_to_c((s_note){intv.n, 0});
    intv.fps = chromatic_distance - intv_sn_dist;
    return intv;
}

int mode_flats(mode_i mode)
{
    if (mode == MINOR) {
        mode = AEOLIAN;
    }
    return 5 - thirds_from_six(mode);
}

int s_note_flats(s_note note)
{
    return thirds_from_six(note.n) - 5 + (note.fps * -7);
}

c_note s_to_c(s_note note)
{
    return positive_modulo(11 + thirds_from_six(note.n) * 5 + note.fps, 12);
}

c_note sintv_to_cintv(s_note sintv)
{
    int oc = sintv.n / 7;
    if (sintv.n < 0) {
        oc = oc - 1; //intervals 0-6 should be octave 0; -6 to -1 are octave -1.
    }
    int sint = positive_modulo(sintv.n, 7);
    return
        positive_modulo(11 + thirds_from_six(sint)*5, 12)
        + oc * 12 + sintv.fps;
}

s_note resolve_chromatic(c_note cn, mode_i mode)
{
    s_note ret;
    int m_flats = mode_flats(mode);
    /* Determine how many flats or sharps in major key of c_note */
    int octave = 0;
    while ((cn + octave * 12) % 5 != 0) {
        octave += 1;
    }
    int key_flats = ((cn + octave * 12) / 5);
    if (key_flats + m_flats > 6) {
        key_flats -= 12;
    } else if ((key_flats + m_flats == 6) && (m_flats <= 0)) {
        key_flats -= 12;
    }

    ret.n  = positive_modulo(S_C + key_flats * 3, 7);
    /* reverse: positive for sharps, negative for flats */
    ret.fps = (key_flats >= 0 ? -1 : 1) * ((abs(key_flats + 2) + 3) / 7);

    return ret;
}

s_note enharm(s_note orig, int direction)
{
    s_note ret = orig;
    if (direction > 0) {
        for (int i = 0; i < direction; i++) {
            ret.n = positive_modulo(ret.n + 1, 7);
            if ((ret.n == S_F) || (ret.n == S_C)) {
                ret.fps -= 1;
            } else {
                ret.fps -= 2;
            }
        }
    } else {
        for (int i = 0; i < abs(direction); i++) {
            ret.n = positive_modulo(ret.n - 1, 7);
            if ((ret.n == S_E) || (ret.n == S_B)) {
                ret.fps += 1;
            } else {
                ret.fps += 2;
            }
        }
    }
    return ret;
}

int Key::flatsharp_limit = FLAT_SHARP_LIMIT;
bool Key::modal = MODAL;

Key::Key(c_note n, mode_i m)
{
    if (n >= 0 && n < 12) {
        this->chrom_n = n;
    }  else {
        throw logic_error("Invalid note given to Key chromatic constructor");
    }
    set_mode(m, false);
    this->staff_n = resolve_chromatic(chrom_n, this->mode);
    _update_repr();
}

Key::Key(s_note sn, mode_i m)
{
    _staff_construct(sn, m);
}

Key::Key(string n)
{
    n.append(" "); //avoid out-of-bounds
    char fc = n.at(0);
    s_note sn;
    if (fc >= 'A' && fc <= 'G') {
        sn.n = positive_modulo(fc - 'A' - 2, 7);
    } else {
        throw logic_error("Note must start with uppercase letter A-G");
    }
    sn.fps = 0;
    mode_i m = 0;
    char nc;
    int i = 1;
    while ((nc = n.at(i)) == 'b' || (nc == '#')) {
        sn.fps += (nc == '#') ? 1 : -1;
        i += 1;
    }
    if (nc == 'm') {
        m = MINOR;
    }
    _staff_construct(sn, m);
}

void Key::_staff_construct(s_note sn, mode_i m)
{
    if (sn.n >= 0 && sn.n < 7) {
        this->staff_n = sn;
    } else {
        throw logic_error("Invalid note given to staff-note constructor of Key");
    }
    this->chrom_n = s_to_c(this->staff_n);
    set_mode(m);
}

s_note Key::interval_in_key(c_note cn)
{
/* first go by fourths to see if cn is the 0, 4, 1, 5, 2, 6, or #3. */
    c_note chrom_i = this->get_chrom_n();
    s_note intv = SN(0, 0);
    /* if major mode, we say sharp three; else flat four */
    int fourths_count =
    (this->get_mode() == IONIAN || this->get_mode() == LYDIAN ||
    this->get_mode() == MIXOLYDIAN) ? 7 : 6;
    for(int i = 0; i < fourths_count; i++) {
        if (cn == chrom_i) {
            return intv;
        }
        chrom_i = (chrom_i + 7) % 12;
        intv.n = (intv.n + 4) % 7;
        if (intv.n == 3) {intv.fps += 1;}
    }
/* It's none of those. Now check by thirds 3, b6, b2, b5, b8, b4.*/
    chrom_i = (this->get_chrom_n() + 5) % 12;
    intv.n = 3;
    intv.fps = 0;
    for (int i = 0; i < (12 - fourths_count); i++) {
        if (cn == chrom_i) {
            return intv;
        }
        chrom_i = (chrom_i + 5) % 12;
        intv.n = (intv.n + 3) % 7;
        if (intv.n == 6) {intv.fps -= 1;}
    }
/* we should have found a name by now */
    throw logic_error("Error in _chrom_construct");
}

mode_i Key::get_mode() const
{
    return this->mode;
}

/* As it is, a Key has no memory of how it is specificed, so if it
 * its enharmonic specification is changed(e.g. from Fb to E), it won't
 * give any preference to the original notation. It will be as if the
 * Key was created given an E. Because of this, changing the mode of an
 * existing key probably isn't a good idea. */
void Key::set_mode(mode_i m, bool update)
{
    if (modal) {
        if (0 <= m && m < 7) {
            this->mode = m;
        } else if (m == MINOR) {
            this->mode = AEOLIAN;
        } else {
            throw logic_error("Invalid mode");
        }
    } else {
        if (m == MINOR || m == MAJOR) {
            this->mode = m;
        } else if (m == AEOLIAN) {
            this->mode = MINOR; //we're not that fancy :)
        } else {
            throw logic_error(
            "Invalid mode(modal is false, so mode must be major or minor)");
        }
    }
    if (update) {
        _update_repr();
    }
}

c_note Key::get_chrom_n() const
{
    return this->chrom_n;
}

s_note Key::get_staff_n() const
{
    return this->staff_n;
}

void Key::_update_repr() {
    int m_flats = mode_flats(this->mode);
    int key_flats = s_note_flats(this->staff_n);

    if (key_flats + m_flats > flatsharp_limit) {
        this->staff_n = enharm(this->staff_n, -1);
        _update_repr(); // is it that bad?
    } else if (key_flats + m_flats < -flatsharp_limit) {
        this->staff_n = enharm(this->staff_n, 1);
        _update_repr();
    }
}

string Key::disp() const
{
    return s_fps_str(this->staff_n);
}

string Key::disp_full() const
{
    string ret = disp();
    ret.append(" ");
    if (modal) {
        ret.append(mode_str(this->mode));
    } else {
        if (mode == MAJOR) {
            ret.append("Major");
        } else if (mode == MINOR || mode == AEOLIAN) {
            ret.append("Minor");
        }
    }
    return ret;
}

int Key::get_flatsharp_limit()
{
    return flatsharp_limit;
}

// probably shouldn't allow this
void Key::set_flatsharp_limit(int lim)
{
    if (lim >= 6) {
        flatsharp_limit = lim;
    } else {
        throw logic_error(
        "Flat-sharp limit must be greater than or equal to 6");
    }
}

bool Key::get_modal()
{
    return modal;
}

// or this
void Key::set_modal(bool m)
{
    if (m) {
        cout << "Setting modal to true: good luck\n";
    }
    modal = m;
}

Key key_from_sharps(int sharps, mode_i mode)
{
    s_note staff_n;
    int major_sharps = sharps + mode_flats(mode);
    staff_n.n = positive_modulo(S_C - 3 * major_sharps, 7);
    staff_n.fps = (major_sharps >= 0 ? 1 : -1) * ((abs(major_sharps - 2) + 3) / 7);
    return Key(staff_n, mode);
}

/* Note functions */
Note::Note(int mn)
{
    this->octave = (mn / 12) - 1;
    _chrom_construct(mn, Key("C"));
}
Note::Note(int mn, Key k)
{
    this->octave = (mn / 12) - 1;
    _chrom_construct(mn, k);
}

Note::Note(c_note cn, int oct)
{
    int mn = 0;
    if (cn > 11 || cn < 0) {
        throw logic_error("tried to create invalid chromatic note");
    } else {
        mn = (oct + 1)*12 + cn;
    }

    this->octave = oct;
    _chrom_construct(mn, Key("C"));
}

Note::Note(c_note cn, int oct, Key k)
{
    int mn = 0;
    if (cn > 11 || cn < 0) {
        throw logic_error("tried to create invalid chromatic note");
    } else {
        mn = (oct + 1)*12 + cn;
    }

    this->octave = oct;
    _chrom_construct(mn, k);
}

/* Currently this assigns flatted values to all accidentals except for
 * the sharp three(in major modes). It will ALWAYS add an accidental to a
 * note in the key, so in the key of Eb it will say that a B is the
 * flat 5, or Cb (If you want B, you have to use one of staff note
 * constructors). It may be smart to modify this to prefer
 * interpretations with fewer flats or sharps. */
void Note::_chrom_construct(int midi_n, Key k)
{
    int chrom_n = midi_n % 12;
/* first go by fourths to see if cn is the 0, 4, 1, 5, 2, 6, or #3. */
    s_note intv = k.interval_in_key(chrom_n);
    this->octave = (midi_n/12) - 1;
    this->staff_n = add_intv(k.get_staff_n(), intv);
}

Note::Note(s_note sn, int oct) : staff_n(sn), octave(oct)
{

}

Note::Note(Key k, int oct, s_note intv)
{
    int mn = (oct+1)*12 + k.get_chrom_n() + sintv_to_cintv(intv);
    this->octave = (mn/12) - 1;
    this->staff_n = add_intv(k.get_staff_n(), intv);
}

Note Note::ctranspose(int c_intv)
{
    return Note(this->get_midi_n() + c_intv);
}


Note Note::ktranspose(Key k_orig, int c_intv)
{
    s_note this_intv = get_intv(k_orig.get_staff_n(), this->staff_n);
    int key_oct = (this->get_midi_n() - sintv_to_cintv(this_intv) + c_intv) / 12 - 1;
    return Note(Key(positive_modulo(k_orig.get_chrom_n() + c_intv, 12), k_orig.get_mode()), key_oct, this_intv);
}

/* TODO: ensure that this works for negative intervals */
Note Note::ktranspose(Key k_orig, s_note intv)
{
    s_note this_intv = get_intv(k_orig.get_staff_n(), this->staff_n);
    int new_key_oct = (get_midi_n() - sintv_to_cintv(this_intv) + sintv_to_cintv(intv)) / 12 -1;
    s_note new_key_sn = add_intv(k_orig.get_staff_n(), intv);
    return Note(Key(new_key_sn, k_orig.get_mode()), new_key_oct, this_intv);
}

Note Note::ktranspose(Key k_orig, Key k_dest, int which)
{
    s_note this_intv = get_intv(k_orig.get_staff_n(), this->staff_n);
    int pos_dist = positive_modulo(k_dest.get_chrom_n() - k_orig.get_chrom_n(), 12);
    int new_key_oct;
    new_key_oct = (this->get_midi_n() - sintv_to_cintv(this_intv) + pos_dist) / 12 - 1 + (which);
    return Note(k_dest, new_key_oct, this_intv);
}

Key Note::to_key(int m)
{
    return Key(this->staff_n, m);
}

int Note::get_midi_n() const
{
    return s_to_c(this->staff_n) + 12*(this->octave + 1);
}

c_note Note::get_chrom_n() const
{
    return s_to_c(this->staff_n);
}

s_note Note::get_staff_n() const
{
    return this->staff_n;
}

int Note::get_octave() const
{
    return this->octave;
}

string Note::disp() const
{
    return s_fps_str(this->staff_n) + std::to_string(this->octave);
}

bool Note::operator<(const Note &Note2)
{
    return this->get_midi_n() < Note2.get_midi_n();
}

bool Note::operator>(const Note &Note2)
{
    return this->get_midi_n() > Note2.get_midi_n();
}

string chord_string(vector<Note> *chord)
{
    string ret = "{" + chord->at(0).disp();
    for (int i = 1; i < chord->size(); i++) {
        ret = ret + ", " + chord->at(i).disp();
    }
    ret = ret + "}";
    return ret;
}
