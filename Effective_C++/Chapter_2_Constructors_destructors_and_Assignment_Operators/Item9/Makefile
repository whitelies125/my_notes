all: example1.exe example2.exe example3.exe example4.exe

example1.exe: example1.cpp
	-g++ --std=c++20 -o example1.exe example1.cpp

example2.exe: example2.cpp
	-g++ --std=c++20 -o example2.exe example2.cpp

example3.exe: example3.cpp
	-g++ --std=c++20 -o example3.exe example3.cpp

example4.exe: example4.cpp
	-g++ --std=c++20 -o example4.exe example4.cpp


.PHONY: run clean example1.exe example2.exe example3.exe example4.exe
run:
	-./example1.exe
	-./example2.exe
	-./example3.exe
	-./example4.exe
example1:
	./example1.exe
example2:
	./example2.exe
example3:
	./example3.exe
example4:
	./example4.exe
clean:
	rm ./*.exe
