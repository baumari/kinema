INCDIR = $(CURDIR)/include
LIBDIR = $(CURDIR)/lib
SRCDIR = $(CURDIR)/src
BINDIR = $(CURDIR)/bin
#CXX = `root-config --cxx`
CXX = clang++
ifeq ($(CXX),root-config --cxx)
	ROOTLIBS = 'root-config --libs'
	CFLAGS = `root-config --cflags` \
	-O3 -W -Wall -Wextra -Wno-long-long \
	-fno-common -I$(INCDIR) -I$(ROOTLIBS) -fPIC -MMD -MP
	LDFLAGS = `root-config --glibs` -lm
else
	CFLAGS = -O3 -W -Wall -Wextra -Wno-long-long \
	-fno-common -I$(INCDIR) -fPIC -MMD -MP
	LDFLAGS = -lm	
endif

TARGET = $(BINDIR)/kinema 
LIB = $(LIBDIR)/mykinema.so

SOURCES = $(wildcard $(SRCDIR)/*.cxx)
OBJS = $(addprefix $(SRCDIR)/, $(notdir $(SOURCES:.cxx=.o)))
DEPS = $(addprefix $(SRCDIR)/, $(notdir $(SOURCES:.cxx=.d)))

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^
	$(CXX) -shared $(OBJS) $(LDFLAGS) -o $(LIB)

$(SRCDIR)/%.o : $(SRCDIR)/%.cxx
	@[ -d $(SRCDIR) ]
	$(CXX) $(CFLAGS) -o $@ -c $<

all: clean $(TARGET)

clean:
	-rm -f $(OBJS) $(DEPS) $(TARGET) $(LIB)

ifneq ($(filter clean,$(MAKECMDGOALS)),clean)
-include $(DEP)
endif

