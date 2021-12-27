DEBUG ?= TRUE
FLAGS += -std=c++14 -MMD -MP -Wall -Wextra

ifdef DEBUG
FLAGS += -O2 -ggdb
CFLAGS += -DDEBUG
else
FLAGS += -O2
CFLAGS += -DNDEBUG
LDFLAGS += -s
endif

CXX ?= c++

COMPILE = $(CXX) $(CFLAGS) $(FLAGS) -c
LINK = $(CXX) $(LDFLAGS) $(FLAGS)

SOURCES = test.cxx timegm.cxx
OBJECTS =  $(SOURCES:.cxx=.o)
DEPENDENCIES = $(OBJECTS:.o=.d)

EXECUTABLE = test

.PHONY: all clean

all: $(EXECUTABLE)
-include $(DEPENDENCIES)

clean:
	-rm -v $(OBJECTS) $(DEPENDENCIES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LINK) -o$@ $^

%.o : %.cxx
	$(COMPILE) -o $@ $<

