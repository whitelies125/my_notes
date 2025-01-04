CXX_FLAGS := -std=c++20

all: main.exe

main.exe: main.o file_system.o directory.o
	g++ $(CXX_FLAGS) -o main.exe main.o file_system.o directory.o

main.o: main.cpp
	g++ $(CXX_FLAGS) -c -o main.o main.cpp

file_system.o: file_system.cpp
	g++ $(CXX_FLAGS) -c -o file_system.o file_system.cpp

directory.o: directory.cpp
	g++ $(CXX_FLAGS) -c -o directory.o directory.cpp

.PHONY: run clean
run:
	./main.exe
clean:
	rm ./*.exe
	rm ./*.o
