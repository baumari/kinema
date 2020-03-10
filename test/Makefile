# General Makefile for c, c++ TEST

INCDIR = $(CURDIR)/../include
LIBDIR = $(CURDIR)/../lib
#SRCDIR = $(CURDIR)/src
#OBJDIR = $(SRCDIR)/obj
#BINDIR = $(CURDIR)/bin
TARGET = test 
#LIBS = $(LIBDIR)/libkinema.so
#LIB = 
SUFFIX = cxx

#CXX = `root-config --cxx`
CXX = g++
#CXX = clang++

#CC =
######################################################
# You should edit above depending on your environment.
######################################################

ifeq ($(CXX),root-config --cxx)
	ROOTLIBS = 'root-config --libs'
	CXXFLAGS = `root-config --cflags` \
	-O3 -W -Wall -Wextra -Wno-long-long \
	-fno-common -I$(INCDIR) -I$(ROOTLIBS) -fPIC 
#-fno-common -I$(INCDIR) -I$(ROOTLIBS) -fPIC -MMD -MP -std=c++11
	LDFLAGS = `root-config --glibs` -lm
else
	CXXFLAGS = -O3 -W -Wall -Wextra -Wno-long-long \
	-fno-common -I$(INCDIR) -fPIC 
#-fno-common -I$(INCDIR) -fPIC -MMD -MP -std=c++11
	LDFLAGS = -lm	
endif

all: 
	$(CXX) -c test.cxx $(CXXFLAGS) -o test.o
	$(CXX) test.o -o test $(LDFLAGS) -L $(LIBDIR) -lkinema
	-rm -f test.o

clean:
	rm -f test
