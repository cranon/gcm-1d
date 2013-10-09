CFLAGS=-c
OBJDIR=src/
OBJDIR1=src/calculators/
SOURCES=$(OBJDIR)Parser.cpp $(OBJDIR)Body.cpp $(OBJDIR)Mesh.cpp $(OBJDIR)Scene.cpp $(OBJDIR)Node.cpp $(OBJDIR1)NumMethod.cpp $(OBJDIR1)Monitor.cpp $(OBJDIR1)Approximator.cpp
OBJECTS=$(SOURCES:.cpp=.o)
MAINFILE=$(OBJDIR)main.cpp

all: run
    
run: $(MAINFILE) $(OBJECTS)
	g++ $(OBJECTS) $(MAINFILE) -o run -O1 -larmadillo

.cpp.o:
	g++ $(CFLAGS) $< -o $@

.PHONY : clean   
clean:   
	-rm run $(OBJECTS)
