/*
 *  Classes to represent musical objects in equal-tempered harmony--
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
#define MINOR -1

using namespace std;

int positive_modulo(int i, int n);

/* string representations of note and mode ints */
string c_note_str(int c);
string s_note_str(int s);
string s_fps_str(int s, int fps);
string mode_str(int m);

int thirds_from_six(int sn);
int add_intv(int base_sn, int base_fps, int intv_sn, int intv_fps, int &res_fps);
int get_intv(int low_sn, int low_fps, int up_sn, int up_fps, int &intv_fps);

/* Determine how many flats are in the mode relative to major. Lydian
 * mode return -1.*/
int mode_flats(int mode);
/* Determine how many flats are in the major mode of the given note(in
 * staff notation).  Sharps are returned as negative numbers.*/
int s_note_flats(int s, int fps);
/* chromatic representation of a staff note */
int s_to_c(int s, int fps);
/* how many half steps in the interval. Intervals are specified starting
 * from zero, so 0 is unison, 1, is a major second, 4 is a perfect fifth,
 * 7 is an octave, etc. */
int sintv_to_cintv(int sintv, int fps);
int resolve_chromatic(int c_note, int mode, int &fps);

/*
 * A Key represents one of the 12 equal tempered notes from
 * the perspective that the note is the key center of some
 * music. It doesn't have an octave, but it does have a
 * mode, which is only used to determine how to notate the Key(e.g. to
 * choose between C# and Db). Keys should be used to specify harmonic
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
     lydian(B) and ionian(F#), flats for all others. Returns the staff note
     number, and sets fps to -1 for flat, 0 for natural, and +1 for sharp on
     that note.

     For darker modes, this can have difficult results- for
     example, in the locrian mode, it will return A#(5 sharps) instead of
     Bb(7 flats). That's why it's recommended to set Key::modal to
     false and only use major/minor. */
    Key(int cn, int m); //chromatic(mod 12) representation, and mode.

    /* Using the following constructors, key will use the given representation
    unless the mode m of the key has more than flatsharp_limit flats or sharps.
    flatsharp_limit is 7 by default and can be changed with static method
    set_flatsharp_limit. */
    Key(int sn, int fs, int m); // 7-note representation, and mode.
    Key(string n);        //string representation
    int get_mode() const;
    /* if update_repr is true, set_mode rechecks the key signature
     * against the flat-sharp limit. */
    void set_mode(int m, bool update_repr=true);
    int get_chrom_n() const;
    int get_staff_n() const; // get flats or sharps with get_fps
    int get_fps() const;
    string disp() const; // just display note name
    string disp_full() const; // include mode
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
    void _staff_construct(int sn, int fs, int m);
    void _update_repr();

    static int flatsharp_limit;
    static bool modal;
    int chrom_n;
    int mode;
    int staff_n;
    int fps; //flats or sharps on staff_n
};

/* represents notes from C0(midi number 12) to G9(midi number 127) */
class Note
{
public:
    Note(int mn); // midi keycode
    Note(int mn, Key k);
    Note(int cn, int oct);
    Note(int cn, int oct, Key k);
    Note(int sn, int fs, int oct);
    Note(int sn, int fs, int oct, Key k);
    Note(Key k, int oct, int intv=0, int fs=0);
    /* transpose chromatically.*/
    Note ctranspose(int c_intv);
    /* transpose via the key center. The only difference is potentially
     * how the new notes are notated.  */
    Note ktranspose(int c_intv);
    Note ktranspose(int s_intv, int fs);
    Note ktranspose(Key k, int which=0);
    int get_midi_n() const;
    int get_chrom_n() const;
    int get_staff_n() const;
    int get_fps() const;
    int get_octave() const;
    Key get_key() const;
    string disp() const;
private:
    void _chrom_construct();
    int midi_n;
    int staff_n;
    int fps;
    int octave;
    Key key;
};

#endif
