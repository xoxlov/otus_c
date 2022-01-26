# compiler name
CC=gcc
# compiling flags here
CFLAGS=-c -Wall
# linking flags here
LFLAGS=-Wall
# instead of typing - get the list of sources and headers with wildcard
SOURCES=$(wildcard *.c)
INCLUDES=$(wildcard *.h)
OBJDIR := obj
OBJECTS := $(SOURCES:.c=.o)
EXECUTABLE=hello.exe
all: $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@
# The ".c.o:" means building ".o" (object files) from ".c" (source files)
.c.o:
#   http://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#   $@ - The file name of the target of the rule.
#   $< - The name of the first prerequisite.
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf *.o $(EXECUTABLE)
