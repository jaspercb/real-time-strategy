CC := g++
# CC = clang --analyze

SRCDIR := src
BUILDDIR := build
TARGET := bin/rts

ccred=$(shell echo "\033[0;31m")
ccbold=$(shell echo "\033[1m")
ccend=$(shell echo "\033[0m")

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++0x -g -O3 -Wall
LIB := -lSDL2 -lSDL2_image -lSDL2_ttf
INC := -I include

$(TARGET): $(OBJECTS)
	@echo "    Linking... $(ccbold)$(TARGET)$(ccend)"; $(CC) $^ -o $(TARGET) $(LIB);

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "    Compiling  $(ccbold)$<$(ccend)"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<;

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket


	.phony clean
