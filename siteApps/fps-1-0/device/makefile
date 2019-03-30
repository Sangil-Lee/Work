################
#reference
################
# https://code.i-harness.com/ko/q/25dc75
################

appname := driver

CXX := g++
CXXFLAGS := -Wall -g

src := $(shell find . -maxdepth 1 -name "*.cpp")
objects := $(patsubst %.cpp, %.o, $(src))
lib := $(shell find . -maxdepth 1 -name "*.h")

all: $(appname)

$(appname):	$(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I $(lib) -o $(appname) $(objects) $(LDLIBS)

#depend: .depend								# don't understand

#.depend: $(src)
#	rm -rf ./.depend
#	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

#time.o: time.cpp
#	$(CXX) -c time.cpp

clean:
	rm -f $(objects) $(appname)

#distclean: clean
#	rm -f $(objects) $(appname)

# include .depend











#if add cpp file?

#driver:	time.o function.o
#	g++ -o driver time.o function.o
#
#time.o: time.cpp
#	g++ -c time.cpp
#
#function.o: function.cpp function.h
#	g++ -c function.cpp

#if used all | clean target?

#all: driver
#clean:
#										?? need to clean grammer
#driver:	time.o function.o
#	g++ -o driver time.o function.o
#
#time.o: time.cpp
#	g++ -c time.cpp
#
#function.o: function.cpp function.h
#	g++ -c function.cpp
