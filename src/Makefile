SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, bin/%.o, $(SOURCES))

FLAGS=-g -O3 -Wno-deprecated-declarations

.PHONY: all flightsim

rts: bin $(OBJECTS)
	g++ -o $@ $(filter-out $(word 1, $^), $^) $(FLAGS) -flto

bin/%.o: %.cpp
	g++ -c $< -o $@ $(FLAGS)

bin:
	@mkdir bin

clean:
	rm -rf bin/