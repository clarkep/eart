#include "synth1.h"
#include "theory.h"
#include "quiz.h"

#include <vector>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;

void disp_vec(vector<s_note> v)
{
    for (int i = 0; i < v.size(); i++) {
        cout << "(" << v.at(i).n << ", " << v.at(i).fps << "), ";
    }
    cout << endl;
}

bool vecs_equal(vector<s_note> v, vector<s_note> w)
{
    if (v.size()!=w.size()) {
        return false;
    }
    for (int i = 0; i < v.size(); i++) {
        if (!s_note_eq(v.at(i), w.at(i))) {
            return false;
        }
    }
    return true;
}

bool streams_equal(istream *stream1, istream *stream2)
{
    string str1, str2;
    do  {
        *stream1 >> str1;
        *stream2 >> str2;
        if (str1 != str2) {
            return false;
        }
    } while (stream1->good() && stream2->good());
    if (stream1->good() || stream2->good()) { // one of them has more in the stream...
        return false;
    } else {
        return true;
    }
}

/* functions in theory.h */
int test_tfs() {
    cout << "Testing thirds_from_six...";
    assert (thirds_from_six(6) == 0);
    assert (thirds_from_six(2) == 1);
    assert (thirds_from_six(5) == 2);
    assert (thirds_from_six(1) == 3);
    assert (thirds_from_six(4) == 4);
    assert (thirds_from_six(0) == 5);
    assert (thirds_from_six(3) == 6);
    cout << "Passed." << endl;
    return 0;
}

int test_add_intv()
{
    cout << "Testing add_intv...";
    s_note res;
    res = add_intv(SN(S_E, -1), SN(1, 0));
    assert (res.n == S_F);
    assert (res.fps == 0);
    res = add_intv(SN(S_E, 0), SN(12, -2));
    assert (res.n == S_C);
    assert (res.fps == -1);
    res = add_intv(SN(S_F, 0), SN(6, -2));
    assert (res.n == S_E);
    assert (res.fps == -2);
    res = add_intv(SN(S_B, 0), SN(10, 0));
    assert (res.n == S_E);
    assert (res.fps == 0);
    res = add_intv(SN(S_B, -1), SN(6, 2));
    assert (res.n == S_A);
    assert (res.fps == 2);
    res = add_intv(SN(S_A, -1), SN(2, -2));
    assert (res.n == S_C);
    assert (res.fps == -2);
    res = add_intv(SN(S_E, -2), SN(8, 1));
    assert (res.n == S_F);
    assert (res.fps == 0);
    res = add_intv(SN(S_D, 0), SN(-3, 0));
    assert (res.n == S_A);
    assert (res.fps == 0);
    res = add_intv(SN(S_F, 1), SN(-5, 0));
    assert (res.n == S_A);
    assert (res.fps == 1);
    res = add_intv(SN(S_G, -1), SN(-5, 0));
    assert (res.n == S_B);
    assert (res.fps == -1);
    res = add_intv(SN(S_C, -1), SN(-1, 1));
    assert (res.n == S_B);
    assert (res.fps == 0);
    cout << "Passed." << endl;
    return 0;
}

int test_get_intv()
{
    cout << "Testing get_intv...";
    s_note res;
    res = get_intv(SN(S_C, 1), SN(S_B, 0));
    assert (res.n == 6);
    assert (res.fps == -1);
    res = get_intv(SN(S_B, 0), SN(S_C, 0));
    assert (res.n == 1);
    assert (res.fps == -1);
    res = get_intv(SN(S_G, 1), SN(S_A, 1));
    assert (res.n == 1);
    assert (res.fps == 0);
    res = get_intv(SN(S_G, 0), SN(S_B, -1));
    assert (res.n == 2);
    assert (res.fps == -1);
    res = get_intv(SN(S_G, 0), SN(S_F, -1));
    assert (res.n == 6);
    assert (res.fps == -2);
    res = get_intv(SN(S_C, -1), SN(S_B, 1));
    assert (res.n == 6);
    assert (res.fps == 2);
    res = get_intv(SN(S_C, 1), SN(S_B, -2));
    assert (res.n == 6);
    assert (res.fps == -3);
    res = get_intv(SN(S_D, 2), SN(S_B, -1));
    assert (res.n == 5);
    assert (res.fps == -3);
    cout << "Passed." << endl;
    return 0;
}

int test_mode_flats()
{
    cout << "Testing mode_flats...";
    assert (mode_flats(LYDIAN) == -1);
    assert (mode_flats(IONIAN) == 0);
    assert (mode_flats(MIXOLYDIAN) == 1);
    assert (mode_flats(DORIAN) == 2);
    assert (mode_flats(AEOLIAN) == 3);
    assert (mode_flats(PHRYGIAN) == 4);
    assert (mode_flats(LOCRIAN) == 5);

    assert (mode_flats(MAJOR) == 0);
    assert (mode_flats(MINOR) == 3);
    cout << "Passed." << endl;
    return 0;
}

int test_s_note_flats()
{
    cout << "Testing s_note_flats...";
    assert (s_note_flats(SN(S_C, 0)) == 0);
    assert (s_note_flats(SN(S_B, 0)) == -5);
    assert (s_note_flats(SN(S_B, -1)) == 2);
    assert (s_note_flats(SN(S_F, 1)) == -6);
    assert (s_note_flats(SN(S_A, 1)) == -10);
    assert (s_note_flats(SN(S_D, -2)) == 12);
    cout << "Passed." << endl;
    return 0;
}

int test_s_to_c() { //and sintv_to_cintv
    cout << "Testing s_to_c and sintv_to_cintv...";
    assert (s_to_c(SN(S_A, 0)) == C_A);
    assert (s_to_c(SN(S_A, 1)) == C_A_SHARP);
    assert (s_to_c(SN(S_A, 2)) == C_B);
    assert (s_to_c(SN(S_F, -2)) == C_E_FLAT);
    assert (s_to_c(SN(S_B, -1)) == C_B_FLAT);
    assert (s_to_c(SN(S_C, -1)) == C_B);
    assert (sintv_to_cintv(SN(4, 1)) == 8);
    assert (sintv_to_cintv(SN(1, -1)) == 1);
    assert (sintv_to_cintv(SN(8, -1)) == 13);
    assert (sintv_to_cintv(SN(6, -1)) == 10);
    assert (sintv_to_cintv(SN(4, 2)) == 9);
    assert (sintv_to_cintv(SN(0, 0)) == 0);
    assert (sintv_to_cintv(SN(-8, 0)) == -13);
    assert (sintv_to_cintv(SN(-4, -1)) == -8);
    assert (sintv_to_cintv(SN(-5, -1)) == -9);
    assert (sintv_to_cintv(SN(0, -1)) == -1);
    cout << "Passed." << endl;
    return 0;
}

int test_resolve_chromatic()
{
    cout << "Testing resolve_chromatic...";
    s_note res;
    res = resolve_chromatic(C_C, IONIAN);
    assert (res.n == S_C);
    assert (res.fps == 0);
    res = resolve_chromatic(C_C, LYDIAN);
    assert (res.n == S_C);
    assert (res.fps == 0);
    res = resolve_chromatic(C_C, LOCRIAN);
    assert (res.n == S_C);
    assert (res.fps == 0);

    res = resolve_chromatic(C_F, LOCRIAN);
    assert (res.n == S_F);
    assert (res.fps == 0);
    res = resolve_chromatic(C_B_FLAT, LOCRIAN);
    assert (res.n == S_A);
    assert (res.fps == 1);
    res = resolve_chromatic(C_B_FLAT, PHRYGIAN);
    assert (res.n == S_B);
    assert (res.fps == -1);
    res = resolve_chromatic(C_E_FLAT, LOCRIAN);
    assert (res.n == S_D);
    assert (res.fps == 1);
    res = resolve_chromatic(C_E_FLAT, PHRYGIAN);
    assert (res.n == S_D);
    assert (res.fps == 1);
    res = resolve_chromatic(C_E_FLAT, AEOLIAN);
    assert (res.n == S_E);
    assert (res.fps == -1);

    res = resolve_chromatic(C_B, LYDIAN);
    assert (res.n == S_B);
    assert (res.fps == 0);
    res = resolve_chromatic(C_F_SHARP, LYDIAN);
    assert (res.n == S_G);
    assert (res.fps == -1);
    res = resolve_chromatic(C_F_SHARP, IONIAN);
    assert (res.n == S_F);
    assert (res.fps == 1);
    res = resolve_chromatic(C_C_SHARP, LYDIAN);
    assert (res.n == S_D);
    assert (res.fps == -1);
    res = resolve_chromatic(C_C_SHARP, IONIAN);
    assert (res.n == S_D);
    assert (res.fps == -1);
    res = resolve_chromatic(C_C_SHARP, MIXOLYDIAN);
    assert (res.n == S_D);
    assert (res.fps = -1);
    res = resolve_chromatic(C_C_SHARP, DORIAN);
    assert (res.n == S_C);
    assert (res.fps = 1);
    cout << "Passed." << endl;
    return 0;
}

int test_from_sharps() {
    cout << "Testing from_sharps..." << endl;
    assert (s_note_eq(from_sharps(-2), SN(6, -1)));
    assert (s_note_eq(from_sharps(3), SN(5, 0)));
    assert (s_note_eq(from_sharps(0), SN(0,0)));
    assert (s_note_eq(from_sharps(-11), SN(5, -2)));
    assert (s_note_eq(from_sharps(-8), SN(3, -1)));
    cout << "Passed." << endl;
    return 0;
}

// TODO: add more test cases?
int test_transpositions()
{
    cout << "Testing tranpositions..." << endl;
    vector<s_note> v;
    vector<s_note> sol{SN(2, -1 ), SN(-1, -1), SN(0, 0), SN(1, 0), SN(-1, 0)};
    v = transpositions(-2, 3, -4, 5);
    assert (vecs_equal(v, sol));
    cout << "Passed." << endl;
    return 0;
}

/* tests for the Key class */

int test_key_chrom_constructor() {
    cout << "Testing chromatic constructor..." << endl;
    ostringstream out;
    for (int i = 0; i < 12; i++) {
        Key k(i, MAJOR);
        Key kmin(i, MINOR);
        out << left << "Testing " << setw(7) << c_note_str(i) + ": "
        << setw(10) << left << k.disp_full() << setw(10) << kmin.disp_full() << endl;
    }
    Key::set_modal(true);
    for (int i = 0; i < 12; i++) {
        out << left << "Testing " << setw(7) << c_note_str(i) + ": ";
        for (int m = 0; m < 7; m++) {
            Key k(i, (3+4*m) % 7);
            out << setw(14) << k.disp_full();
        }
        out << endl;
    }

    istringstream stream1(out.str());
    ifstream stream2("testing/chrom_constructor.txt");
    assert(streams_equal(&stream1, &stream2));
    cout << "Passed." << endl;
}

int test_key_staff_constructor() {
    cout << "Testing staff constructor..." << endl;
    /* this was messed with during the change to s_note, might need a rewrite*/
    ostringstream out;
    /* flats */
    Key::set_modal(true);
    Key::set_flatsharp_limit(7);
    s_note sn = SN(S_C, 0);
    for (int i = 0; i < 20; i++) {
        sn.n = (S_C + i * 3);
        if ((sn.n % 7) == S_B) {
            sn.fps -= 1;
        }
        out << left << "Testing " << setw(7) << s_fps_str(SN(sn.n % 7, sn.fps)) + ": ";
        for (mode_i m = 0; m < 7; m++) {
            Key k(SN(sn.n % 7, sn.fps), (3+4*m) % 7);
            out << setw(15) << k.disp_full();
        }
        out << endl;
    }
    /* sharps */
    out << endl;
    sn.fps = 0;
    for (int i = 0; i < 20; i++) {
        sn.n = (S_C - i * 3);
        if (positive_modulo(sn.n, 7) == S_F) {
            sn.fps += 1;
        }
        out << left << "Testing " << setw(7) << s_fps_str(SN(positive_modulo(sn.n, 7), sn.fps)) + ": ";
        for (int m = 0; m < 7; m++) {
            Key k(SN(positive_modulo(sn.n, 7), sn.fps), (3+4*m) % 7);
            out << setw(15) << k.disp_full();
        }
        out << endl;
    }

    istringstream stream1(out.str());
    ifstream stream2("testing/staff_constructor.txt");
    assert (streams_equal(&stream1, &stream2));
    cout << "Passed." << endl;
}

int test_key_text_constructor() {
    cout << "Testing text constructor..." << endl;
    ostringstream out;
    Key::set_modal(false);
    Key k("B");
    out << k.disp_full() << endl;
    Key g("Gbm");
    out << g.disp_full() << endl;
    g = Key("G#m");
    out << g.disp_full() << endl;
    g = Key("Cbbbm");
    out << g.disp_full() << endl;

    istringstream stream1(out.str());
    ifstream stream2("testing/text_constructor.txt");
    assert (streams_equal(&stream1, &stream2));
    cout << "Passed." << endl;
    return 0;
}

int test_key_misc()
{

}

/* tests for the note class */

int test_note_chrom_constructor()
{
    cout << "Testing chromatic constructor..." << endl;
    Key::set_modal(true);
    Key::set_flatsharp_limit(11);
    Note n = Note(70);
    assert(n.get_chrom_n() == C_B_FLAT);
    assert(s_note_eq(n.get_staff_n(), SN(S_B, -1)));
    n = Note(C_F_SHARP, 4);
    assert (n.get_midi_n() == 66);
    assert (s_note_eq(n.get_staff_n(), SN(S_F, 1)));
    n = Note(C_F_SHARP, 2, Key(SN(S_C, 0), LYDIAN));
    assert (n.get_midi_n() == 42);
    assert (s_note_eq(n.get_staff_n(), SN(S_F, 1)));
    n = Note(C_F_SHARP, 2, Key(SN(S_C, 0), AEOLIAN));
    assert (n.get_midi_n() == 42);
    assert (s_note_eq(n.get_staff_n(), SN(S_G, -1)));
    n = Note(C_D, 7, Key(SN(S_E, -1), IONIAN));
    assert (n.get_midi_n() == 98);
    assert (s_note_eq(n.get_staff_n(), SN(S_D, 0)));
    n = Note(C_B, 4, Key(SN(S_E, -1), IONIAN));
    assert (n.get_midi_n() == 71);
    assert (s_note_eq(n.get_staff_n(), SN(S_C, -1)));
    /*random test cases*/
    n = Note(C_G, 6, Key(SN(S_E, 0), PHRYGIAN));
    assert (n.get_midi_n() == 91);
    assert (s_note_eq(n.get_staff_n(), SN(S_G, 0)));
    n = Note(C_B, 4, Key(SN(S_B, 0), IONIAN));
    assert (n.get_midi_n() == 71);
    assert (s_note_eq(n.get_staff_n(), SN(S_B, 0)));
    n = Note(C_D, 2, Key(SN(S_A, 0), IONIAN));
    assert (n.get_midi_n() == 38);
    assert (s_note_eq(n.get_staff_n(), SN(S_D, 0)));
    n = Note(C_G_FLAT, 4, Key(SN(S_B, 0), LYDIAN));
    assert (n.get_midi_n() == 66);
    assert (s_note_eq(n.get_staff_n(), SN(S_F, 1)));
    n = Note(C_A, 0, Key(SN(S_E, -1), DORIAN));
    assert (n.get_midi_n() == 21);
    assert (s_note_eq(n.get_staff_n(), SN(S_B, -2)));

    cout << "Passed." << endl;
    return 0;
    /* edge cases */
}

int test_note_staff_constructor()
{
    cout << "Testing staff constructor..." << endl;
    Key::set_modal(true);
    Note n = Note(SN(S_C, -1), 4);
    assert (n.get_midi_n() == 71);
    assert (n.get_chrom_n() == C_B);
    n = Note(SN(S_C, -1), 4);
    assert (n.get_midi_n() == 71);
    assert (n.get_chrom_n() == C_B);
    n = Note(SN(S_D, 1), 1);
    assert (n.get_midi_n() == 27);
    assert (n.get_chrom_n() == C_D_SHARP);
    n = Note(SN(S_C, -1), 0);
    assert (n.get_midi_n() == 23);
    assert (n.get_chrom_n() == C_B);
    assert (n.get_octave() == 0);
    //n = Note(S_B, 1, -1);

    cout << "Passed." << endl;
    return 0;
}

int test_note_intv_constructor()
{
    cout << "Testing interval constructor..." << endl;
    /* random tests */
    Key::set_modal(true);
    Note n = Note(Key("C"), 4, SN(4, 1));
    assert (s_note_eq(n.get_staff_n(), SN(S_G, 1)));
    assert (n.get_midi_n() == 68);
    assert (n.get_chrom_n() == C_G_SHARP);
    n = Note(Key("Ab"), 4, SN(2, -1));
    assert (s_note_eq(n.get_staff_n(), SN(S_C, -1)));
    assert (n.get_midi_n() == 71);
    n = Note(Key("Fm"), 1, SN(6, -2));
    assert (s_note_eq(n.get_staff_n(), SN(S_E, -2)));
    assert(n.get_midi_n() == 38);
    assert(n.get_chrom_n() == C_D);
    n = Note(Key("E"), 3, SN(9, -2));
    assert (s_note_eq(n.get_staff_n(), SN(S_G, -1)));
    assert (n.get_midi_n() == 66);
    n = Note(Key("Eb"), 7, SN(8, 0));
    assert (s_note_eq(n.get_staff_n(), SN(S_F, 0)));
    assert (n.get_midi_n() == 113);
    assert (n.get_chrom_n() == C_F);
    n = Note(Key(SN(S_E, 0), PHRYGIAN), 3, SN(1, -1));
    assert (s_note_eq(n.get_staff_n(), SN(S_F, 0)));
    assert (n.get_midi_n() == 53);
    n = Note(Key("Db"), 5, SN(5, 2));
    assert (s_note_eq(n.get_staff_n(), SN(S_B, 1)));
    assert (n.get_midi_n() == 84);
    assert (n.get_chrom_n() == C_C);
    assert (n.get_octave() == 6);

    cout << "Passed." << endl;
    return 0;
}

int test_note_ctranspose()
{
    cout << "Testing chromatic transpose..." << endl;
    Note n = Note(120);
    n = n.ctranspose(7);
    assert (n.get_midi_n() == 127);
    assert (s_note_eq(n.get_staff_n(), SN(S_G, 0)));
    n = n.ctranspose(-13);
    assert (n.get_midi_n() == 114);
    assert (s_note_eq(n.get_staff_n(), SN(S_F, 1)));

    cout << "Passed." << endl;
    return 0;
}

int test_note_ktranspose()
{
    cout << "Testing key transpose..." << endl;
    Key::set_flatsharp_limit(9);
    Note n = Note(Key("C"), 6, SN(2, 0));
    assert (s_note_eq(n.get_staff_n(), SN(S_E, 0)));
    assert (n.get_midi_n() == 88);
    n = n.ktranspose(Key("C"), -4); // into key of Ab bc Ab has less accidentals than G#
    assert (s_note_eq(n.get_staff_n(), SN(S_C, 0)));
    assert (n.get_midi_n() == 84);
    assert (n.get_octave() == 6);
    n = n.ktranspose(Key("Ab"), SN(4, 1)); // into key of E
    assert (s_note_eq(n.get_staff_n(), SN(S_G, 1)));
    assert (n.get_midi_n() == 92);
    n = n.ktranspose(Key("E"), SN(-8, 0)); // into key of D#, which has 9 sharps.
    assert (s_note_eq(n.get_staff_n(), SN(S_F, 2))); // 2 of D# is F##
    assert (n.get_midi_n() == 79);
    n = n.ktranspose(Key("D#"), SN(4, 0)); // now into key of Bb, because A# is above fs limit
    assert (s_note_eq(n.get_staff_n(), SN(S_D, 0)));
    assert (n.get_midi_n() == 86);
    assert (n.get_octave() == 6);

    /* and now for my next trick... */
    Note m = Note(Key("F#"), 2, SN(-3, -1)); //this is the b4 of F#, which is C.
    assert (s_note_eq(m.get_staff_n(), SN(S_C, 0)));
    assert (m.get_midi_n() == 36);
    assert (m.get_octave() == 2);
    m = m.ktranspose(Key("F#"), Key("Gb"));
    assert (s_note_eq(m.get_staff_n(), SN(S_D, -2)));
    assert (m.get_midi_n() == 36);
    assert (m.get_octave() == 2);
    m = m.ktranspose(Key("Gb"), Key("A"), 0);
    assert (s_note_eq(m.get_staff_n(), SN(S_E, -1)));
    assert (m.get_midi_n() == 39);
    m = m.ktranspose(Key("A"), Key("B"), 1);
    assert (s_note_eq(m.get_staff_n(), SN(S_F, 0)));
    assert (m.get_midi_n() == 53);

    cout << "Passed." << endl;
    return 0;
}

/* tests for functions in the file quiz.cpp */
// TODO: I tested a few edge cases, but needs more and consistent tests.

int man_test_transpose_q()
{
    int seed = 1340;
    srand(seed);
    NoteSynth synth = NoteSynth();
    vector<Note> chord{Note(C_D, 3), Note(C_F, 3), Note(C_A, 3)}; //Cmaj
    ChordQItem c = {chord, Key("Dm"), "min"};
    for (int n=0; n < 10; n++) {
        vector<ChordQItem> res = transpose_q(vector<ChordQItem>{c}, 50, 59, -6, 6, true);
        //for (int i=0; i < res.notevec.size(); i++) {
        //    cout << res.notevec[i].disp() << " " << res.notevec[i].get_midi_n() << endl;
        //}
        cout << res.at(0).key.disp() << res.at(0).suffix << endl;
        cout << chord_string(res.at(0).notevec) << endl;
        synth.play_chord(res.at(0).notevec);
    }
}


int test_free_functions()
{
    cout << "----------------------------------------------------------------" << endl;
    cout << "Testing theory.cpp helper functions" << endl;
    cout << "----------------------------------------------------------------" << endl << endl;
    test_tfs();
    test_add_intv();
    test_get_intv();
    test_mode_flats();
    test_s_note_flats();
    test_s_to_c();
    test_resolve_chromatic();
    test_from_sharps();
    test_transpositions();
}

int test_key()
{
    cout << "----------------------------------------------------------------" << endl;
    cout << "Testing Key class methods" << endl;
    cout << "----------------------------------------------------------------" << endl << endl;
    test_key_chrom_constructor();
    test_key_staff_constructor();
    test_key_text_constructor();
}

int test_note()
{
    cout << "----------------------------------------------------------------" << endl;
    cout << "Testing Note class methods" << endl;
    cout << "----------------------------------------------------------------" << endl << endl;
    test_note_chrom_constructor();
    test_note_staff_constructor();
    test_note_intv_constructor();
    test_note_ctranspose();
    test_note_ktranspose();
}


int test_misc()
{
    Key k;
}

int main(int argc, char** argv)
{
    man_test_transpose_q();

    //test_misc();
    test_free_functions();
    test_key();
    test_note();

    cout << "----------------------------------------------------------------" << endl;
    cout << "All tests passed.\n";
}
