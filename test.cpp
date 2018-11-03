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
    assert (thirds_from_six(6) == 0);
    assert (thirds_from_six(2) == 1);
    assert (thirds_from_six(5) == 2);
    assert (thirds_from_six(1) == 3);
    assert (thirds_from_six(4) == 4);
    assert (thirds_from_six(0) == 5);
    assert (thirds_from_six(3) == 6);
    return 0;
}

int test_add_intv()
{
  int fps;
  assert (add_intv(S_E, -1, 1, 0, fps) == S_F);
  assert (fps == 0);
  assert (add_intv(S_E, 0, 12, -2, fps) == S_C);
  assert (fps == -1);
  assert (add_intv(S_F, 0, 6, -2, fps) == S_E);
  assert (fps == -2);
  assert (add_intv(S_B, 0, 10, 0, fps) == S_E);
  assert (fps == 0);
  assert (add_intv(S_B, -1, 6, 2, fps) == S_A);
  assert (fps == 2);
  assert (add_intv(S_A, -1, 2, -2, fps) == S_C);
  assert (fps == -2);
  assert (add_intv(S_E, -2, 8, 1, fps) == S_F);
  assert (fps == 0);
  assert (add_intv(S_D, 0, -3, 0, fps) == S_A);
  assert (fps == 0);
  assert (add_intv(S_F, 1, -5, 0, fps) == S_A);
  assert (fps == 1);
  assert (add_intv(S_G, -1, -5, 0, fps) == S_B);
  assert (fps == -1);
  assert (add_intv(S_C, -1, -1, 1, fps) == S_B);
  assert (fps == 0);
  return 0;
}

int test_get_intv()
{
    int fps;
    assert (get_intv(S_C, 1, S_B, 0, fps) == 6);
    assert (fps == -1);
    assert (get_intv(S_G, 1, S_A, 1, fps) == 1);
    assert (fps == 0);
    assert (get_intv(S_G, 0, S_B, -1, fps) == 2);
    assert (fps == -1);
    assert (get_intv(S_G, 0, S_F, -1, fps) == 6);
    assert (fps == -2);
    assert (get_intv(S_C, -1, S_B, 1, fps) == 6);
    assert (fps == 2);
    assert (get_intv(S_C, 1, S_B, -2, fps) == 6);
    assert (fps == -3);
    assert (get_intv(S_D, 2, S_B, -1, fps) == 5);
    assert (fps == -3);
    return 0;
}

int test_mode_flats()
{
    assert (mode_flats(LYDIAN) == -1);
    assert (mode_flats(IONIAN) == 0);
    assert (mode_flats(MIXOLYDIAN) == 1);
    assert (mode_flats(DORIAN) == 2);
    assert (mode_flats(AEOLIAN) == 3);
    assert (mode_flats(PHRYGIAN) == 4);
    assert (mode_flats(LOCRIAN) == 5);

    assert (mode_flats(MAJOR) == 0);
    assert (mode_flats(MINOR) == 3);
    return 0;
}

int test_s_note_flats()
{
    assert (s_note_flats(S_C, 0) == 0);
    assert (s_note_flats(S_B, 0) == -5);
    assert (s_note_flats(S_B, -1) == 2);
    assert (s_note_flats(S_F, 1) == -6);
    assert (s_note_flats(S_A, 1) == -10);
    assert (s_note_flats(S_D, -2) == 12);
    return 0;
}

int test_s_to_c() { //and sintv_to_cintv
    assert (s_to_c(S_A, 0) == C_A);
    assert (s_to_c(S_A, 1) == C_A_SHARP);
    assert (s_to_c(S_A, 2) == C_B);
    assert (s_to_c(S_F, -2) == C_E_FLAT);
    assert (s_to_c(S_B, -1) == C_B_FLAT);
    assert (sintv_to_cintv(4, 1) == 8);
    assert (sintv_to_cintv(1, -1) == 1);
    assert (sintv_to_cintv(8, -1) == 13);
    assert (sintv_to_cintv(6, -1) == 10);
    assert (sintv_to_cintv(4, 2) == 9);
    assert (sintv_to_cintv(0, 0) == 0);
    assert (sintv_to_cintv(-8, 0) == -13);
    assert (sintv_to_cintv(-4, -1) == -8);
    assert (sintv_to_cintv(-5, -1) == -9);
    return 0;
}

int test_resolve_chromatic()
{
    int fps;
    assert (resolve_chromatic(C_C, IONIAN, fps) == S_C);
    assert (fps == 0);
    assert (resolve_chromatic(C_C, LYDIAN, fps) == S_C);
    assert (fps == 0);
    assert (resolve_chromatic(C_C, LOCRIAN, fps) == S_C);
    assert (fps == 0);

    assert (resolve_chromatic(C_F, LOCRIAN, fps) == S_F);
    assert (fps == 0);
    assert (resolve_chromatic(C_B_FLAT, LOCRIAN, fps) == S_A);
    assert (fps == 1);
    assert (resolve_chromatic(C_B_FLAT, PHRYGIAN, fps) == S_B);
    assert (fps == -1);
    assert (resolve_chromatic(C_E_FLAT, LOCRIAN, fps) == S_D);
    assert (fps == 1);
    assert (resolve_chromatic(C_E_FLAT, PHRYGIAN, fps) == S_D);
    assert (fps == 1);
    assert (resolve_chromatic(C_E_FLAT, AEOLIAN, fps) == S_E);
    assert (fps == -1);

    assert (resolve_chromatic(C_B, LYDIAN, fps) == S_B);
    assert (fps == 0);
    assert (resolve_chromatic(C_F_SHARP, LYDIAN, fps) == S_G);
    assert (fps == -1);
    assert (resolve_chromatic(C_F_SHARP, IONIAN, fps) == S_F);
    assert (fps == 1);
    assert (resolve_chromatic(C_C_SHARP, LYDIAN, fps) == S_D);
    assert (fps == -1);
    assert (resolve_chromatic(C_C_SHARP, IONIAN, fps) == S_D);
    assert (fps == -1);
    assert (resolve_chromatic(C_C_SHARP, MIXOLYDIAN, fps) == S_D);
    assert (fps = -1);
    assert (resolve_chromatic(C_C_SHARP, DORIAN, fps) == S_C);
    assert (fps = 1);
    return 0;
}

/* tests for the Key class */

int test_key_chrom_constructor() {
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
}

int test_key_staff_constructor() {
    ostringstream out;
    /* flats */
    Key::set_modal(true);
    Key::set_flatsharp_limit(7);
    int s_note = S_C;
    int fps = 0;
    for (int i = 0; i < 20; i++) {
        s_note = (S_C + i * 3);
        if ((s_note % 7) == S_B) {
            fps -= 1;
        }
        out << left << "Testing " << setw(7) << s_fps_str(s_note % 7, fps) + ": ";
        for (int m = 0; m < 7; m++) {
            Key k(s_note % 7, fps, (3+4*m) % 7);
            out << setw(15) << k.disp_full();
        }
        out << endl;
    }
    /* sharps */
    out << endl;
    fps = 0;
    for (int i = 0; i < 20; i++) {
        s_note = (S_C - i * 3);
        if (positive_modulo(s_note, 7) == S_F) {
            fps += 1;
        }
        out << left << "Testing " << setw(7) << s_fps_str(positive_modulo(s_note, 7), fps) + ": ";
        for (int m = 0; m < 7; m++) {
            Key k(positive_modulo(s_note, 7), fps, (3+4*m) % 7);
            out << setw(15) << k.disp_full();
        }
        out << endl;
    }

    istringstream stream1(out.str());
    ifstream stream2("testing/staff_constructor.txt");
    assert (streams_equal(&stream1, &stream2));
}

int test_key_text_constructor() {
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
}

int test_key_from_sharps()
{
    Key::set_modal(true);
    Key::set_flatsharp_limit(20);
    Key k = key_from_sharps(2, LYDIAN);
    assert (k.get_staff_n() == S_G);
    assert (k.get_fps() == 0);
    assert (k.get_mode() == LYDIAN);
    k = key_from_sharps(-8, AEOLIAN);
    assert (k.get_staff_n() == S_D);
    assert (k.get_fps() == -1);
    assert (k.get_mode() == AEOLIAN);
    k = key_from_sharps(8, LYDIAN);
    assert (k.get_staff_n() == S_C);
    assert (k.get_fps() == 1);
    assert (k.get_mode() == LYDIAN);
    k = key_from_sharps(-15, MAJOR);
    assert (k.get_staff_n() == S_F);
    assert (k.get_fps() == -2);
}

int test_key_misc()
{

}

/* tests for the note class */

int test_note_chrom_constructor()
{
    Key::set_modal(true);
    Key::set_flatsharp_limit(11);
    Note n = Note(70);
    assert(n.get_chrom_n() == C_B_FLAT);
    assert(n.get_staff_n() == S_B);
    assert(n.get_fps() == -1);
    n = Note(C_F_SHARP, 4);
    assert (n.get_midi_n() == 66);
    assert (n.get_staff_n() == S_F);
    assert (n.get_fps() == 1);
    n = Note(C_F_SHARP, 2, Key(S_C, 0, LYDIAN));
    assert (n.get_midi_n() == 42);
    assert (n.get_staff_n() == S_F);
    assert (n.get_fps() == 1);
    n = Note(C_F_SHARP, 2, Key(S_C, 0, AEOLIAN));
    assert (n.get_midi_n() == 42);
    assert (n.get_staff_n() == S_G);
    assert (n.get_fps() == -1);
    n = Note(C_D, 7, Key(S_E, -1, IONIAN));
    assert (n.get_midi_n() == 98);
    assert (n.get_staff_n() == S_D);
    assert (n.get_fps() == 0);
    n = Note(C_B, 4, Key(S_E, -1, IONIAN));
    assert (n.get_midi_n() == 71);
    assert (n.get_staff_n() == S_C);
    assert (n.get_fps() == -1);
    /*random test cases*/
    n = Note(C_G, 6, Key(S_E, 0, PHRYGIAN));
    assert (n.get_midi_n() == 91);
    assert (n.get_staff_n() == S_G);
    assert (n.get_fps() == 0);
    n = Note(C_B, 4, Key(S_B, 0, IONIAN));
    assert (n.get_midi_n() == 71);
    assert (n.get_staff_n() == S_B);
    assert (n.get_fps() == 0);
    n = Note(C_D, 2, Key(S_A, 0, IONIAN));
    assert (n.get_midi_n() == 38);
    assert (n.get_staff_n() == S_D);
    assert (n.get_fps() == 0);
    n = Note(C_G_FLAT, 4, Key(S_B, 0, LYDIAN));
    assert (n.get_midi_n() == 66);
    assert (n.get_staff_n() == S_F);
    assert (n.get_fps() == 1);
    n = Note(C_A, 0, Key(S_E, -1, DORIAN));
    assert (n.get_midi_n() == 21);
    assert (n.get_staff_n() == S_B);
    assert (n.get_fps() == -2);
    return 0;
    /* edge cases */
}

int test_note_staff_constructor()
{
    Key::set_modal(true);
    Note n = Note(S_C, -1, 4, Key("Gb"));
    assert (n.get_midi_n() == 71);
    assert (n.get_chrom_n() == C_B);
    n = Note(S_C, -1, 4);
    assert (n.get_midi_n() == 71);
    assert (n.get_chrom_n() == C_B);
    n = Note(S_D, 1, 1);
    assert (n.get_midi_n() == 27);
    assert (n.get_chrom_n() == C_D_SHARP);
    n = Note(S_C, -1, 0);
    assert (n.get_midi_n() == 23);
    assert (n.get_chrom_n() == C_B);
    assert (n.get_octave() == 0);
    //n = Note(S_B, 1, -1);
}

int test_note_intv_constructor()
{
    /* random tests */
    Key::set_modal(true);
    Note n = Note(Key("C"), 4, 4, 1);
    assert (n.get_staff_n() == S_G);
    assert (n.get_fps() == 1);
    assert (n.get_midi_n() == 68);
    assert (n.get_chrom_n() == C_G_SHARP);
    n = Note(Key("Ab"), 4, 2, -1);
    assert (n.get_staff_n() == S_C);
    assert (n.get_fps() == -1);
    assert (n.get_midi_n() == 71);
    n = Note(Key("Fm"), 1, 6, -2);
    assert (n.get_staff_n() == S_E);
    assert (n.get_fps() == -2);
    assert(n.get_midi_n() == 38);
    assert(n.get_chrom_n() == C_D);
    n = Note(Key("E"), 3, 9, -2);
    assert (n.get_staff_n() == S_G);
    assert (n.get_fps() == -1);
    assert (n.get_midi_n() == 66);
    n = Note(Key("Eb"), 7, 8, 0);
    assert (n.get_staff_n() == S_F);
    assert (n.get_fps() == 0);
    assert (n.get_midi_n() == 113);
    assert (n.get_chrom_n() == C_F);
    n = Note(Key(S_E, 0, PHRYGIAN), 3, 1, -1);
    assert (n.get_staff_n() == S_F);
    assert (n.get_fps() == 0);
    assert (n.get_midi_n() == 53);
    n = Note(Key("Db"), 5, 5, 2);
    assert (n.get_staff_n() == S_B);
    assert (n.get_fps() == 1);
    assert (n.get_midi_n() == 84);
    assert (n.get_chrom_n() == C_C);
    assert (n.get_octave() == 6);
    return 0;
}

int test_note_ctranspose()
{
  Note n = Note(120);
  n = n.ctranspose(7);
  assert (n.get_midi_n() == 127);
  assert (n.get_staff_n() == S_G);
  assert (n.get_fps() == 0);
  n = n.ctranspose(-13);
  assert (n.get_midi_n() == 114);
  assert (n.get_staff_n() == S_F);
  assert (n.get_fps() == 1);
  return 0;
}

int test_note_ktranspose()
{
    Key::set_flatsharp_limit(9);
    Note n = Note(Key("C"), 6, 2, 0);
    assert (n.get_staff_n() == S_E);
    assert (n.get_fps() == 0);
    assert (n.get_midi_n() == 88);
    n = n.ktranspose(-4); // into key of Ab bc Ab has less accidentals than G#
    assert (n.get_staff_n() == S_C);
    assert (n.get_fps() == 0);
    assert (n.get_midi_n() == 84);
    assert (n.get_octave() == 6);
    n = n.ktranspose(4, 1); // into key of E
    assert (n.get_staff_n() == S_G);
    assert (n.get_fps() == 1);
    assert (n.get_midi_n() == 92);
    n = n.ktranspose(-8, 0); // into key of D#, which has 9 sharps.
    assert (n.get_staff_n() == S_F); // 2 of D# is F##
    assert (n.get_fps() == 2);
    assert (n.get_midi_n() == 79);
    n = n.ktranspose(4, 0); // now into key of Bb, because A# is above fs limit
    assert (n.get_key().get_staff_n() == S_B);
    assert (n.get_key().get_fps() == -1);
    assert (n.get_staff_n() == S_D);
    assert (n.get_fps() == 0);
    assert (n.get_midi_n() == 86);
    assert (n.get_octave() == 6);

    /* and now for my next trick... */
    Note m = Note(Key("F#"), 2, -3, -1); //this is the b4 of F#, which is C.
    assert (m.get_staff_n() == S_C);
    assert (m.get_fps() == 0);
    assert (m.get_midi_n() == 36);
    assert (m.get_octave() == 2);
    m = m.ktranspose(Key("Gb"));
    assert (m.get_staff_n() == S_D);
    assert (m.get_fps() == -2);
    assert (m.get_midi_n() == 36);
    assert (m.get_octave() == 2);
    m = m.ktranspose(Key("A"), 1);
    assert (m.get_staff_n() == S_E);
    assert (m.get_fps() == -1);
    assert (m.get_midi_n() == 39);
    m = m.ktranspose(Key("B"), 2);
    assert (m.get_staff_n() == S_F);
    assert (m.get_fps() == 0);
    assert (m.get_midi_n() == 53);
}

/* tests for functions in the file quiz.cpp */

int man_test_transpose_q()
{
    NoteSynth synth = NoteSynth();
    vector<Note> chord{Note(C_C, 4), Note(C_E, 4), Note(C_G, 4), Note(C_B, 4)}; //Cmaj7
    for (int n=0; n < 100; n++) {
    ChordQItem res = transpose_q(chord, 30, 88, "maj7", 7, false);
    //for (int i=0; i < res.notevec.size(); i++) {
    //    cout << res.notevec[i].disp() << " " << res.notevec[i].get_midi_n() << endl;
    //}
    cout << res.name << endl;
    synth.play_chord(res.notevec);
}
}

int test_free_functions()
{
    cout << "----------------------------------------------------------------" << endl;
    cout << "Testing theory.cpp helper functions" << endl;
    cout << "----------------------------------------------------------------" << endl << endl;
    cout << "Testing thirds_from_six...";
    test_tfs();
    cout << "passed." << endl;
    cout << "Testing add_intv...";
    test_add_intv();
    cout << "passed." << endl;
    cout << "Testing get_intv...";
    test_get_intv();
    cout << "passed." << endl;
    cout << "Testing mode_flats...";
    test_mode_flats();
    cout << "passed." << endl;
    cout << "Testing s_note_flats...";
    test_s_note_flats();
    cout << "passed." << endl;
    cout << "Testing s_to_c and sintv_to_cintv...";
    test_s_to_c();
    cout << "passed." << endl;
    cout << "Testing resolve_chromatic...";
    test_resolve_chromatic();
    cout << "passed." << endl << endl;
}

int test_key()
{
    cout << "----------------------------------------------------------------" << endl;
    cout << "Testing Key class methods" << endl;
    cout << "----------------------------------------------------------------" << endl << endl;
    cout << "Testing chromatic constructor..." << endl;
    test_key_chrom_constructor();
    cout << endl << "Test passed. ";
    cout << "Testing staff constructor..." << endl;
    test_key_staff_constructor();
    cout << endl << "Test passed. ";
    cout << "Testing text constructor..." << endl;
    test_key_text_constructor();
    cout << "Test passed." << endl;
    cout << "Testing key_from_sharps..." << endl;
    test_key_from_sharps();
    cout << "Test passed." << endl << endl;
}

int test_note()
{
    cout << "----------------------------------------------------------------" << endl;
    cout << "Testing Note class methods" << endl;
    cout << "----------------------------------------------------------------" << endl << endl;
    cout << "Testing chromatic constructor..." << endl;
    test_note_chrom_constructor();
    cout << endl << "Test passed. ";
    cout << "Testing staff constructor..." << endl;
    test_note_staff_constructor();
    cout << endl << "Test passed. ";
    cout << "Testing interval constructor..." << endl;
    test_note_intv_constructor();
    cout << endl << "Test passed. ";
    cout << "Testing chromatic transpose..." << endl;
    test_note_ctranspose();
    cout << endl << "Test passed. ";
    cout << "Testing key transpose..." << endl;
    test_note_ktranspose();
    cout << "Test passed." << endl << endl;
}



int test_misc()
{

}

int main(int argc, char** argv)
{
    man_test_transpose_q();
    /*
    test_misc();
    test_free_functions();
    test_key();
    test_note();
    /*
    cout << "----------------------------------------------------------------" << endl;
    cout << "All tests passed.\n";
    */
}
