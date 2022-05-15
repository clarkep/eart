objects = main.o interface.o synth1.o quiz.o theory.o file_q.o list_q.o

flags = -g 

libraries = -lfluidsynth 

all: heart

heart : $(objects)
	g++ $(flags) -o heart $(objects) $(libraries)

test : test.o $(objects)
	g++ $(flags) -o test test.o $(objects) $(libraries)

%.o : %.cpp
	g++ $(flags) -c -o $@ $< 

clean:
	rm heart test main.o test.o $(objects)
