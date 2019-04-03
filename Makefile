objects = theory.o quiz.o synth1.o

flags = -std=c++11 -g3

libraries = -lfluidsynth

eart : main.o $(objects)
	g++ $(flags) -o eart main.o $(objects) $(libraries)

test : test.o theory.o quiz.o synth1.o
	g++ $(flags) -o test test.o $(objects) $(libraries)

main.o : main.cpp theory.h  quiz.h synth1.h
	g++ $(flags) -c main.cpp

test.o : test.cpp theory.h quiz.h synth1.h
	g++ $(flags) -c test.cpp

theory.o : theory.cpp theory.h
	g++ $(flags) -c theory.cpp

quiz.o : quiz.cpp quiz.h theory.h
	g++ $(flags) -c quiz.cpp

synth1.o : synth1.cpp synth1.h theory.h
	g++ $(flags) -c synth1.cpp

clean:
	rm eart test main.o test.o $(objects)
