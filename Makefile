objects = src/Parser.o src/Scene.o src/Body.o src/Mesh.o src/Node.o src/NumMethod.o src/Monitor.o

all: run

run: src/main.cpp $(objects)
	g++ $(objects) src/main.cpp -o run

Parser.o: src/Parser.cpp src/Parser.h
	g++ -c src/Parser.cpp src/Parser.h -o src/Parser.o

Scene.o: src/Scene.cpp src/Scene.h
	g++ -c src/Scene.cpp src/Scene.h -o src/Scene.o

Body.o: src/Body.cpp src/Body.h
	g++ -c src/Body.cpp src/Body.h -o src/Body.o

Mesh.o: src/Mesh.cpp src/Mesh.h
	g++ -c src/Mesh.cpp src/Mesh.h -o src/Mesh.o
	
Node.o: src/Node.cpp src/Node.h
	g++ -c src/Node.cpp src/Node.h -o src/Node.o
	
NumMethod.o: src/NumMethod.cpp src/NumMethod.h
	g++ -c src/NumMethod.cpp src/NumMethod.h -o src/NumMethod.o
	
Monitor.o: src/Monitor.cpp src/Monitor.h
	g++ -c src/Monitor.cpp src/Monitor.h -o src/Monitor.o
	
.PHONY : clean   
clean:   
	-rm run $(objects)
