SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, bin/%.o, $(SOURCES))

FLAGS=-std=c++0x -g -O3

.PHONY: all rts

rts: bin $(OBJECTS)
	g++ -o $@ $(filter-out $(word 1, $^), $^) $(FLAGS) -flto

bin/%.o: %.cpp
	g++ -c  $< -o $@ $(FLAGS)

bin:
	@mkdir bin

clean:
	rm -rf bin/
