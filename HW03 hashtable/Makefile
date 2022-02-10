# compiler name
CC := gcc
# compiling flags here
CFLAGS := -c
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpedantic
CFLAGS += -std=c11
CFLAGS += -ggdb
# linking flags here
LFLAGS := -Wall

# get the list of sources and headers
SOURCES := $(wildcard *.c)
INCLUDES := $(wildcard *.h)

OBJDIR := obj
OBJECTS := $(SOURCES:.c=.o)

EXECUTABLE := hashtable
CFLAGS += -DPROGRAM_NAME=\"$(EXECUTABLE)\"

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

# building object files from source files
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
