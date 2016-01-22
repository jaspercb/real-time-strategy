SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, ../bin/%.o, $(SOURCES))

FLAGS=-std=c++0x -g -O3

.PHONY: rts clean

rts: ../bin/rts

../bin/rts: bin $(OBJECTS)
	g++ -o $@ $(filter-out $(word 1, $^), $^) $(FLAGS) -flto

../bin/%.o: %.cpp
	g++ -c  $< -o $@ $(FLAGS)

bin:
	@mkdir -p ../bin

clean:
	rm -rf ../bin
