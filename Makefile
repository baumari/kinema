INCDIR = $(CURDIR)/include
LIBDIR = $(CURDIR)/lib
SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/src/obj
BINDIR = $(CURDIR)/bin
TARGET = $(BINDIR)/kinema 
LIB = $(LIBDIR)/libkinema.so
#LIB = 

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

SOURCES = $(wildcard $(SRCDIR)/*.cxx)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cxx=.o)))
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS) $(LIB)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx
	-mkdir -p $(OBJDIR)
	$(CXX) $(CFLAGS) -o $@ -c $<

$(LIB):
	$(CXX) -shared $(OBJS) $(LDFLAGS) -o $@

all: clean $(TARGET)

clean:
	-rm -f $(OBJS) $(DEPS) $(TARGET) $(LIB)

ifneq ($(filter clean,$(MAKECMDGOALS)),clean)
-include $(DEPS)
endif

