INCDIR = $(CURDIR)/include
LIBDIR = $(CURDIR)/lib
#CXX = `root-config --cxx`
CXX = clang++
ifeq ($(CXX),root-config --cxx)
	ROOTLIBS = 'root-config --libs'
	CFLAGS = `root-config --cflags` \
	-O3 -W -Wall -Wextra -Wno-long-long \
	-fno-common -I$(INCDIR) -I$(ROOTLIBS) -fPIC
	LDFLAGS = `root-config --glibs` -lm
else
	CFLAGS = -O3 -W -Wall -Wextra -Wno-long-long \
	-fno-common -I$(INCDIR) -fPIC
	LDFLAGS = -lm	
endif

TARGET = ./bin/kinema 
LIB = ./lib/mykinema.so
SOURCES = $(wildcard ./src/*.cxx)
OBJS = $(addprefix ./src/, $(notdir $(SOURCES:.cxx=.o)))

#.SUFFIXES: .cxx .o

all: $(TARGET)
	rm -f ./src/*.o

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^
	$(CXX) -shared $(OBJS) $(LDFLAGS) -o $(LIB)

#.cxx.o:
#	$(CXX) -c $(CFLAGS) $<

./src/%.o : ./src/%.cxx
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	rm -f ./src/*.o *~ $(TARGET) $(LIB)
#	echo $(OBJS)

