INCDIR = $(CURDIR)/include
LIBDIR = $(CURDIR)/lib
CXX = `root-config --cxx`
ROOTLIBS = 'root-config --libs'
CFLAGS = `root-config --cflags` \
	-O3 -W -Wall -Wextra -Wno-long-long \
	-fno-common -I$(INCDIR) -I$(ROOTLIBS)

LDFLAGS = `root-config --glibs` -lGeom -lm

TARGET = ./bin/kinema 
LIB = ./lib/mykinema.so
SOURCES = $(wildcard ./src/*.cxx)
OBJS = $(addprefix ./src/, $(notdir $(SOURCES:.cxx=.o)))

#.SUFFIXES: .cxx .o

all: $(TARGET)
	rm -f ./src/*.o

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^
	$(CXX) -shared -fPIC $(OBJS) $(LDFLAGS) -o $(LIB)

#.cxx.o:
#	$(CXX) -c $(CFLAGS) $<

./src/%.o : ./src/%.cxx
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o *~ $(TARGET) $(LIB)
#	echo $(OBJS)

