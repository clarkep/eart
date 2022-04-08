objects = interface.o synth1.o quiz.o theory.o file_q.o list_q.o

flags = -g

libraries = -lfluidsynth 

eart : main.o $(objects)
	g++ $(flags) -o eart main.o $(objects) $(libraries)

test : test.o $(objects)
	g++ $(flags) -o test test.o $(objects) $(libraries)

main.o : main.cpp theory.h  quiz.h synth1.h interface.h file_q.h
	g++ $(flags) -c main.cpp

test.o : test.cpp theory.h quiz.h synth1.h interface.h file_q.h
	g++ $(flags) -c test.cpp

interface.o: interface.cpp interface.h quiz.h synth1.h
	g++ $(flags) -c interface.cpp

synth1.o : synth1.cpp synth1.h theory.h
	g++ $(flags) -c synth1.cpp

quiz.o : quiz.cpp quiz.h theory.h synth1.h
	g++ $(flags) -c quiz.cpp

theory.o : theory.cpp theory.h
	g++ $(flags) -c theory.cpp

file_q.o: file_q.cpp file_q.h quiz.h interface.h theory.h
	g++ $(flags) -c file_q.cpp

clean:
	rm eart test main.o test.o $(objects)
