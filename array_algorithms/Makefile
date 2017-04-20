GMOCK_INCLUDE=$(HOME)/usr/include/gmock
GTEST_INCLUDE=$(HOME)/usr/include/gtest
GMOCK_LIB=$(HOME)/usr/lib
GTEST_LIB=$(HOME)/usr/lib

CC=g++
CXX=g++

INCLUDE=-I${GMOCK_INCLUDE}/include -I${GTEST_INCLUDE}/include
CFLAGS=-c -std=c++14 -O3
LDFLAGS=-L${GMOCK_LIB} -L${GTEST_LIB}

SOURCES=array_algorithms.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=array_algorithms

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(INCLUDE) $(CFLAGS) $< -o $@

clean:
	rm *.o main
