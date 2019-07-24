# General Makefile for c, c++
# Sources are must be in ./src
# Headers are must be in ./include
# Other file or directory are made automatically.

INCDIR = $(CURDIR)/include
LIBDIR = $(CURDIR)/lib
SRCDIR = $(CURDIR)/src
OBJDIR = $(SRCDIR)/obj
BINDIR = $(CURDIR)/bin
TARGET = $(BINDIR)/kinema 
LIB = $(LIBDIR)/libkinema.so
#LIB = 
SUFFIX = cxx

#CXX = `root-config --cxx`
#CXX = g++
CXX = clang++

#CC =
######################################################
# You should edit above depending on your environment.
######################################################

ifeq ($(CXX),root-config --cxx)
	ROOTLIBS = 'root-config --libs'
	CXXFLAGS = `root-config --cflags` \
	-O3 -W -Wall -Wextra -Wno-long-long \
	-fno-common -I$(INCDIR) -I$(ROOTLIBS) -fPIC -MMD -MP
#	-fno-common -I$(INCDIR) -I$(ROOTLIBS) -fPIC -MMD -MP -std=c++11
	LDFLAGS = `root-config --glibs` -lm
else
	CXXFLAGS = -O3 -W -Wall -Wextra -Wno-long-long \
	-fno-common -I$(INCDIR) -fPIC -MMD -MP
#	-fno-common -I$(INCDIR) -fPIC -MMD -MP -std=c++11
	LDFLAGS = -lm	
endif

SOURCES = $(wildcard $(SRCDIR)/*.$(SUFFIX))
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cxx=.o)))
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS) $(LIB)
	-mkdir -p $(BINDIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.$(SUFFIX)
	-mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(LIB):
	-mkdir -p $(LIBDIR)
	$(CXX) -shared $(OBJS) $(LDFLAGS) -o $(LIB)

all: clean $(TARGET)

clean:
	-rm -f $(OBJS) $(DEPS) $(TARGET) $(LIB)
	-rm -rf $(BINDIR) $(OBJDIR) $(LIBDIR) 

ifneq ($(filter clean,$(MAKECMDGOALS)),clean)
-include $(DEPS)
endif

