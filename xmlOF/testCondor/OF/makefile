#makefile for the test Objective Functions.

all: testOF testOFF

OPTIONS= -g -Wall -static
#OPTIONS= -O3

testOF: testOF.cpp
	g++ $(OPTIONS) -o testOF testOF.cpp

testOFF: testOF.cpp
	g++ $(OPTIONS) -o testOFF -DFAILURE testOF.cpp

clean:
	rm testOF
	rm testOFF
	rm *~
