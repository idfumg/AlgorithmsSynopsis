GMOCK_INCLUDE=$(HOME)/usr/include/gmock
GTEST_INCLUDE=$(HOME)/usr/include/gtest
GMOCK_LIB=$(HOME)/usr/lib
GTEST_LIB=$(HOME)/usr/lib

CC=clang++
CXX=clang++

INCLUDE=-I${GMOCK_INCLUDE}/include -I${GTEST_INCLUDE}/include
CFLAGS=-c -std=c++11
LDFLAGS=-L${GMOCK_LIB} -L${GTEST_LIB}

SOURCES=linked_list_empty.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=linked_list_empty

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(INCLUDE) $(CFLAGS) $< -o $@

clean:
	rm *.o main
