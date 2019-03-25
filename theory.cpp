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


/* https://stackoverflow.com/questions/14997165 */
int positive_modulo(int i, int n) {
    int tmp = i % n;
    return tmp ? i >= 0 ? tmp : tmp + n : 0;
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

string s_fps_str(int s, int fps) {
    string ret = s_note_str(s);
    for (int i=0; i < abs(fps); i++) {
        ret.append((fps > 0) ? "#" : "b");
    }
    return ret;
}

string c_note_str(int c) {
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

string mode_str(int m) {
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

int add_intv(int base_sn, int base_fps, int intv_sn, int intv_fps, int &res_fps)
{
    int key_sharps = -s_note_flats(base_sn, base_fps);
    int nec = thirds_from_six(positive_modulo(intv_sn, 7)); //fourths from a major 7th, the first note to get a sharp.
    int res_sn = positive_modulo(base_sn + intv_sn, 7);
    res_fps = floor((float) key_sharps / (float) 7) + ((positive_modulo(key_sharps, 7)>=(nec + 1)) ? 1 : 0) + intv_fps;
    return res_sn;
}

int get_intv(int low_sn, int low_fps, int up_sn, int up_fps, int &intv_fps)
{
    int chromatic_distance = positive_modulo(s_to_c(up_sn, 0) - s_to_c(low_sn, 0), 12) - low_fps + up_fps;
    int intv_sn = positive_modulo(up_sn - low_sn, 7);
    int intv_sn_dist = s_to_c(intv_sn, 0);
    intv_fps = chromatic_distance - intv_sn_dist;
    return intv_sn;
}

int mode_flats(int mode) {
    if (mode == MINOR) {
        mode = AEOLIAN;
    }
    return 5 - thirds_from_six(mode);
}

int s_note_flats(int s, int fps)
{
    return thirds_from_six(s) - 5 + (fps * -7);
}

int s_to_c(int s, int fps)
{
    return positive_modulo(11 + thirds_from_six(s) * 5 + fps, 12);
}

int sintv_to_cintv(int sintv, int fps)
{
    int oc = sintv / 7;
    if (sintv < 0) {
        oc = oc - 1; //intervals 0-6 should be octave 0; -6 to -1 are octave -1.
    }
    int sint = positive_modulo(sintv, 7);
    return
        positive_modulo(11 + thirds_from_six(sint)*5, 12)
        + oc * 12 + fps;
}

int resolve_chromatic(int c_note, int mode, int &fps)
{
    int m_flats = mode_flats(mode);
    /* Determine how many flats or sharps in major key of c_note */
    int octave = 0;
    while ((c_note + octave * 12) % 5 != 0) {
        octave += 1;
    }
    int key_flats = ((c_note + octave * 12) / 5);
    if (key_flats + m_flats > 6) {
        key_flats -= 12;
    } else if ((key_flats + m_flats == 6) && (m_flats <= 0)) {
        key_flats -= 12;
    }

    int ret  = positive_modulo(S_C + key_flats * 3, 7);
    /* reverse: positive for sharps, negative for flats */
    fps = (key_flats >= 0 ? -1 : 1) * ((abs(key_flats + 2) + 3) / 7);

    return ret;
}

int Key::flatsharp_limit = FLAT_SHARP_LIMIT;
bool Key::modal = MODAL;

Key::Key(int n, int m)
{
    if (n >= 0 && n < 12) {
        this->chrom_n = n;
    }  else {
        throw logic_error("Invalid note given to Key chromatic constructor");
    }
    set_mode(m, false);
    this->staff_n = resolve_chromatic(chrom_n, mode, this->fps);
    _update_repr();
}

Key::Key(int sn, int fs, int m)
{
    _staff_construct(sn, fs, m);
}

Key::Key(string n)
{
    n.append(" "); //avoid out-of-bounds
    char fc = n.at(0);
    int sn;
    if (fc >= 'A' && fc <= 'G') {
        sn = positive_modulo(fc - 'A' - 2, 7);
    } else {
        throw logic_error("Note must start with uppercase letter A-G");
    }
    int fs = 0;
    int m = 0;
    char nc;
    int i = 1;
    while ((nc = n.at(i)) == 'b' || (nc == '#')) {
        fs += (nc == '#') ? 1 : -1;
        i += 1;
    }
    if (nc == 'm') {
        m = MINOR;
    }
    _staff_construct(sn, fs, m);
}

void Key::_staff_construct(int sn, int fs, int m)
{
    if (sn >= 0 && sn < 7) {
        this->staff_n = sn;
        this->fps = fs;
    } else {
        throw logic_error("Invalid note given to staff-note constructor of Key");
    }
    this->chrom_n = s_to_c(this->staff_n, this->fps);
    set_mode(m);
}


int Key::get_mode() const
{
    return this->mode;
}

/* As it is, a Key has no memory of how it is specificed, so if it
 * its enharmonic specification is changed(e.g. from Fb to E), it won't
 * give any preference to the original notation. It will be as if the
 * Key was created given an E. Because of this, changing the mode of an
 * existing key probably isn't a good idea. */
void Key::set_mode(int m, bool update)
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

int Key::get_chrom_n() const
{
    return this->chrom_n;
}

int Key::get_staff_n() const
{
    return this->staff_n;
}

int Key::get_fps() const
{
    return this->fps;
}

void Key::_update_repr() {
    int m_flats = mode_flats(this->mode);
    int key_flats = s_note_flats(this->staff_n, this->fps);

    if (key_flats + m_flats > flatsharp_limit) {
        this->staff_n = positive_modulo(this->staff_n - 1, 7);
        if (this->staff_n == S_B || this->staff_n == S_E) {
            this->fps += 1;
        } else {
            this->fps += 2;
        }
        _update_repr(); // is it that bad?
    } else if (key_flats + m_flats < -flatsharp_limit) {
        this->staff_n = positive_modulo(this->staff_n + 1, 7);
        if (this->staff_n == S_F || this->staff_n == S_C) {
            this->fps -= 1;
        } else {
            this->fps -= 2;
        }
        _update_repr();
    }
}

string Key::disp() const
{
    return s_fps_str(this->staff_n, this->fps);
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

Key key_from_sharps(int sharps, int mode)
{
    int major_sharps = sharps + mode_flats(mode);
    int staff_n = positive_modulo(S_C - 3 * major_sharps, 7);
    int fps = (major_sharps >= 0 ? 1 : -1) * ((abs(major_sharps - 2) + 3) / 7);
    return Key(staff_n, fps, mode);
}

/* Note functions */
Note::Note(int mn) : key(Key("C"))
{
    int oc = -1;
    if (mn > 127 || mn < 12) {
        throw logic_error("midi note number is out of range(12-127)");
    } else {
        oc = (mn / 12) - 1;
    }
    this->octave = oc;
    _chrom_construct(mn);
}
Note::Note(int mn, Key k) : key(k)
{
    int oc = -1;
    if (mn > 127 || mn < 12) {
        throw logic_error("midi note number is out of range(12-127)");
    } else {
        oc = (mn / 12) - 1;
    }
    this->octave = oc;
    _chrom_construct(mn);
}

Note::Note(int cn, int oct) : key(Key("C"))
{
    int mn = 0;
    if (cn > 11 || cn < 0) {
        throw logic_error("tried to create invalid chromatic note");
    } else {
        mn = (oct + 1)*12 + cn;
    }

    if (mn > 127 || mn < 12) {
        throw logic_error("tried to create note out of range");
    } else {
        this->octave = oct;
        _chrom_construct(mn);
    }
}

Note::Note(int cn, int oct, Key k) : key(k)
{
    int mn = 0;
    if (cn > 11 || cn < 0) {
        throw logic_error("tried to create invalid chromatic note");
    } else {
        mn = (oct + 1)*12 + cn;
    }

    if (mn > 127 || mn < 12) {
        throw logic_error("tried to create note out of range");
    } else {
        this->octave = oct;
        _chrom_construct(mn);
    }
}

/* Currently this assigns flatted values to all accidentals except for
 * the sharp three(in major modes). It will ALWAYS add an accidental to a
 * note in the key, so in the key of Eb it will say that a B is the
 * flat 5, or Cb (If you want B, you have to use one of staff note
 * constructors). It may be smart to modify this to prefer
 * interpretations with fewer flats or sharps. */
void Note::_chrom_construct(int midi_n)
{
    int chrom_n = midi_n % 12;
/* first go by fourths to see if cn is the 0, 4, 1, 5, 2, 6, or #3. */
    int chrom_i = this->key.get_chrom_n();
    int staff_i = this->key.get_staff_n();
    int fps_i = this->key.get_fps();
    /* if major mode, we say sharp three; else flat four */
    int fourths_count =
    (this->key.get_mode() == IONIAN || this->key.get_mode() == LYDIAN ||
    this->key.get_mode() == MIXOLYDIAN) ? 7 : 6;
    for(int i = 0; i < fourths_count; i++) {
        if (chrom_n == chrom_i) {
            this->staff_n = staff_i;
            this->fps = fps_i;
            return;
        }
        chrom_i = (chrom_i + 7) % 12;
        staff_i = (staff_i + 4) % 7;
        if (staff_i == S_F) {fps_i += 1;}
    }
/* It's none of those. Now check by thirds 3, b6, b2, b5, b8, b4.*/
    chrom_i = (this->key.get_chrom_n() + 5) % 12;
    staff_i = (this->key.get_staff_n() + 3) % 7;
    fps_i = (staff_i == S_B) ? this->key.get_fps() + 1 : this->key.get_fps();
    for (int i = 0; i < (12 - fourths_count); i++) {
        if (chrom_n == chrom_i) {
            this->staff_n = staff_i;
            this->fps = fps_i;
            return;
        }
        chrom_i = (chrom_i + 5) % 12;
        staff_i = (staff_i + 3) % 7;
        if (staff_i == S_B) {fps_i -= 1;}
    }
/* we should have found a name by now */
    throw logic_error("Error in _chrom_construct");
}

Note::Note(int sn, int fs, int oct) : staff_n(sn), fps(fs), octave(oct), key(Key("C"))
{
    int mn = (oct +  1)*12 + s_to_c(staff_n, fps);
    if (mn>127 || mn<12) {
        throw logic_error("Note staff constructor: note out of bounds");
    }
}

Note::Note(int sn, int fs, int oct, Key k) : staff_n(sn), fps(fs), octave(oct), key(k)
{
    int mn = (oct + 1)*12 + s_to_c(staff_n, fps);
    if (mn>127 || mn<12) {
        throw logic_error("Note staff constructor: note out of bounds");
    }
}

Note::Note(Key k, int oct, int intv, int fs) : key(k)
{
    int mn = (oct+1)*12 + key.get_chrom_n() + sintv_to_cintv(intv, fs);
    if (mn>127 || mn<12) {
        throw logic_error("Note interval constructor: note out of bounds");
    } else {
        this->octave = (mn/12) - 1;
    }
    this->staff_n = add_intv(key.get_staff_n(), key.get_fps(), intv, fs, this->fps);
}

Note Note::ctranspose(int c_intv)
{
    return Note(this->get_midi_n() + c_intv, this->key);
}


Note Note::ktranspose(int c_intv)
{
    int intv_fs = 0;
    int this_intv = get_intv(this->key.get_staff_n(), this->key.get_fps(), this->staff_n, this->fps, intv_fs);
    int key_oct = (this->get_midi_n() - sintv_to_cintv(this_intv, intv_fs) + c_intv) / 12 - 1;

    return Note(Key(positive_modulo(this->key.get_chrom_n() + c_intv, 12), this->key.get_mode()), key_oct, this_intv, intv_fs);
}

/* TODO: ensure that this works for negative intervals */
Note Note::ktranspose(int s_intv, int fs)
{
    int this_fs = 0;
    int this_intv = get_intv(this->key.get_staff_n(), this->key.get_fps(), this->staff_n, this->fps, this_fs);
    int new_key_oct = (get_midi_n() - sintv_to_cintv(this_intv, this_fs) + sintv_to_cintv(s_intv, fs)) / 12 -1;
    int new_key_fs = 0;
    int new_key_sn = add_intv(this->key.get_staff_n(), this->key.get_fps(), s_intv, fs, new_key_fs);
    return Note(Key(new_key_sn, new_key_fs, this->key.get_mode()), new_key_oct, this_intv, this_fs);
}

Note Note::ktranspose(Key k, int which)
{
    int this_fs = 0;
    int this_intv = get_intv(this->key.get_staff_n(), this->key.get_fps(), this->staff_n, this->fps, this_fs);
    int pos_dist = positive_modulo(k.get_chrom_n() - this->key.get_chrom_n(), 12);
    int new_key_oct;
    if (which > 0) {
        new_key_oct = (this->get_midi_n() - sintv_to_cintv(this_intv, this_fs) + pos_dist) / 12 - 1 + (which - 1);
    } else if (which < 0) {
        new_key_oct = (this->get_midi_n() - sintv_to_cintv(this_intv, this_fs) - 12 + pos_dist) / 12 - 1 + (which + 1);
    } else if (which == 0) {
        new_key_oct = (this->get_midi_n() - sintv_to_cintv(this_intv, this_fs) +
            ((pos_dist <= 6) ? (pos_dist) : (-12 + pos_dist))) / 12 -1;
    }
    return Note(k, new_key_oct, this_intv, this_fs);
}

Key Note::to_key(int m)
{
    return Key(this->staff_n, this->fps, m);
}

int Note::get_midi_n() const
{
    return s_to_c(this->staff_n, this->fps) + 12*(this->octave + 1);
}

int Note::get_chrom_n() const
{
    return s_to_c(this->staff_n, this->fps);
}

int Note::get_staff_n() const
{
    return this->staff_n;
}

int Note::get_fps() const
{
    return this->fps;
}

int Note::get_octave() const
{
    return this->octave;
}

Key Note::get_key() const
{
    return this->key;
}

string Note::disp() const
{
    return s_fps_str(this->staff_n, this->fps) + std::to_string(this->octave);
}

bool Note::operator<(const Note &Note2)
{
    return this->get_midi_n() < Note2.get_midi_n();
}

bool Note::operator>(const Note &Note2)
{
    return this->get_midi_n() > Note2.get_midi_n();
}
