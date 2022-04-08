/*
 *  theory.h
 *  Classes to represent basic musical objects in equal-tempered harmony--
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
 *
 */
#ifndef __THEORY_H_INCLUDED__
#define __THEORY_H_INCLUDED__

#include <string>
#include <vector>

#define FLAT_SHARP_LIMIT 7
#define MODAL 0

/* seven-note (staff) note numbers */
#define S_C 0
#define S_D 1
#define S_E 2
#define S_F 3
#define S_G 4
#define S_A 5
#define S_B 6

/* chromatic note numbers */
#define C_C 0
#define C_C_SHARP 1
#define C_D_FLAT 1
#define C_D 2
#define C_D_SHARP 3
#define C_E_FLAT 3
#define C_E 4
#define C_F 5
#define C_F_SHARP 6
#define C_G_FLAT 6
#define C_G 7
#define C_G_SHARP 8
#define C_A_FLAT 8
#define C_A 9
#define C_A_SHARP 10
#define C_B_FLAT 10
#define C_B 11

/* modes */
#define IONIAN 0
#define DORIAN 1
#define PHRYGIAN 2
#define LYDIAN 3
#define MIXOLYDIAN 4
#define AEOLIAN 5
#define LOCRIAN 6

#define MAJOR 0
#define MINOR 5

#define SN(A, B) (s_note){(A), (B)}

typedef int c_note;
typedef struct s_note {
    int n;
    int fps;
} s_note;
typedef int mode_i;

bool s_note_eq(s_note a, s_note b);

int positive_modulo(int i, int n);
int floor_divide(int x, int y);

/* string representations of note and mode ints */
std::string c_note_str(c_note c);
std::string s_note_str(int s);
std::string s_fps_str(s_note s);
std::string sintv_str(s_note sintv);
std::string mode_str(mode_i m);

int thirds_from_six(int sn);
s_note add_intv(s_note base, s_note intv);
s_note get_intv(s_note lower, s_note upper);

/* Determine how many flats are in the mode relative to major. Lydian
 * mode return -1.*/
int mode_flats(mode_i m);
/* Determine how many flats are in the major mode of the given note(in
 * staff notation).  Sharps are returned as negative numbers.*/
int s_note_flats(s_note note);
/* chromatic representation of a staff note */
c_note s_to_c(s_note note);
/* how many half steps in the interval. Intervals are specified starting
 * from zero, so 0 is unison, 1, is a major second, 4 is a perfect fifth,
 * 7 is an octave, etc. */
c_note sintv_to_cintv(s_note sintv);
s_note resolve_chromatic(c_note cn, mode_i mode);

/* return the (lowest positive) interval that adds num_sharps sharps to a key
   signature when transposing. For examples from_sharps(3)==SN(5, 0)*/
s_note from_sharps(int num_sharps);

s_note enharm(int direction);

class Note;

/*
 * A Key represents one of the 12 equal tempered notes from
 * the perspective that the note is the key center of some
 * music. It doesn't have an octave, but it does have a
 * mode. Keys should be used to specify harmonic
 * centers- for example, the roots of the chords to a song.
 *
 * If modal is set to false, which it is by default, the Key can only
 * be either major or minor. If modal is true, the Key
 * can take any of the seven modern modes of the major scale.
 */
class Key
{
public:
     /* Using  the chromatic constructor, Key will find the enharmonic of cn
    whose given mode has the fewest flats or sharps. Ties go to sharps for
    lydian(B lydian is chosen over Cb lydian) and ionian(F# ionian is chosen
    over Gb ionian), flats for all others.

     For darker modes, this can have difficult results- for
     example, in the locrian mode, it will return A#(5 sharps) instead of
     Bb(7 flats). That's why it's usually better to set Key::modal to
     false and only use major/minor. */
    Key(c_note cn, mode_i m); //chromatic representation, and mode.

    /* Using the following constructors, key will use the given representation
    unless the mode m of the key has more than flatsharp_limit flats or sharps.
    flatsharp_limit is 7 by default and can be changed with static method
    set_flatsharp_limit. */
    Key(s_note sn, mode_i m); // 7-note representation, and mode.
    Key(std::string n="C");       //string representation

    /* given any chromatic note, find an interval that the note makes with this
    key.  */
    s_note interpret_in_key(c_note cn);

    Key interval_key(s_note intv, mode_i m) const;
    Note interval_note(int octave, s_note intv);
    /* return the nth note in the the key's scale */
    Note scale_note(int octave, int scale_n);

    mode_i get_mode() const;
    /* if update_repr is true, set_mode rechecks the key signature
     * against the flat-sharp limit. */
    void set_mode(mode_i m, bool update_repr=true);
    c_note get_chrom_n() const;
    s_note get_staff_n() const;
    std::string disp() const; // just display note name
    std::string disp_full() const; // include mode
    /* The maximum flats or sharps before converting to an enharmonic key. By
       default the limit is 7, and it cannot go lower than that number.
       set_flatsharp_limit does not affect Keys which have already been
       initialized. */
    static int get_flatsharp_limit();
    static void set_flatsharp_limit(int lim);
    /* If modal is set to true, a key can take any of the modes of the major
       scale. By default, when modal is false, a key can only be MAJOR or
       MINOR(ie. IONIAN or AEOLIAN). Keys are represented by the macros defined
       above. */
    static bool get_modal();
    static void set_modal(bool m);
private:
    void _staff_construct(s_note sn, mode_i);
    void _update_repr();

    static int flatsharp_limit;
    static bool modal;
    c_note chrom_n;
    mode_i mode;
    s_note staff_n;
};

/* represents notes from C0(midi number 12) to G9(midi number 127) */
class Note
{
public:
    Note(std::string s);
    Note(int mn); // midi keycode
    Note(int mn, Key k);
    Note(c_note cn, int oct);
    Note(c_note cn, int oct, Key k);
    Note(s_note, int oct);
    Note(Key k, int oct, s_note intv = (s_note){0, 0});
    /* transpose chromatically.*/
    Note ctranspose(c_note c_intv) const;
    /* transpose via the key center. The relationship between the note and the
       key center stays the same. The only difference between ktranspose and
       ctranspose is potentially how the new notes are notated.
       move the key center up by c_intv half steps, choosing the enharmonic
       with the least flats/sharps. */
    Note ktranspose(Key k_orig, c_note c_intv) const;
    Note ktranspose(Key k_orig, s_note sintv) const;
    /* transpose to the new Key k. If which is 0, tranpose up to the nearest
       octave, if its 1, transpose an octave obove that, etc. If which is -1, transpose
       down to the nearest octave, if -2, an octave below that, etc */
    Note ktranspose(Key k_orig, Key k_dest, int which=0) const;
    // return the key centered on this note's value. m is the mode of the new Key.
    Key to_key(int m);
    int get_midi_n() const;
    c_note get_chrom_n() const;
    s_note get_staff_n() const;
    int get_octave() const;
    std::string disp() const;
    bool operator<(const Note &Note2) const;
    bool operator>(const Note &Note2) const;
private:
    void _chrom_construct(int mn, Key k);
    s_note staff_n;
    int octave;
};

class Chord {
public:
    Chord(std::vector<Note> nv={}, Key k=Key("C")) : notevec(nv), key(k) {}
    Chord transpose(s_note intv) const;
    Note get_min() const;
    Note get_max() const;
    std::string to_string() const;
    std::vector<Note> notevec;
    Key key;
};


Chord major_triad(Note n);
Chord minor_triad(Note n);


/* transpositions: generates all possible intervals by which to transpose a key
   such that the tranposition doesn't shift notes up or down by more than
   [uprange, downrange] semitones, and it doesn't add more flats or sharps
   to the key signature than [flat_limit, sharp_limit]. Includes the zero
   interval, SN(0, 0).*/
std::vector<s_note> transpositions(int down_range, int up_range, int flat_limit, int sharp_limit);



#endif
