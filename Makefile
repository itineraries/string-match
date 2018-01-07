CPPCC=g++
CPPFLAGS=-Wall -Werror -O2 -std=c++11
CCC=gcc
CFLAGS=-Wall -Werror -O2

test.o: libmatcher.so test.c
	$(CCC) $(CFLAGS) -o test.o test.c -L. -lmatcher

libmatcher.so: matcher.o
	$(CPPCC) $(CPPFLAGS) -shared -Wl,-soname,libmatcher.so -o libmatcher.so matcher.o

matcher.o: matcher.cpp matcher.h
	$(CPPCC) $(CPPFLAGS) -c -fPIC -o matcher.o matcher.cpp

test: test.o
	python3 test.py; echo
	LD_LIBRARY_PATH=. ./test.o; echo

clean:
	rm matcher.o libmatcher.so test.o; true
