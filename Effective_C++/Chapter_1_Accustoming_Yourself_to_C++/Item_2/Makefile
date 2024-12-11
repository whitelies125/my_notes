all: main.exe

main.exe: main.cpp gameplayer.cpp
	g++ --std=c++20 -o main.exe main.cpp gameplayer.cpp

.PHONY: run clean
run:
	./main.exe
clean:
	rm ./*.exe
