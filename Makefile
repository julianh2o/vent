# Instructions on writting Makefiles:
# https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

# Compiler to use:
CC = g++

# Compile-time flags:
#CFLAGS = -g -Wall -std=c++11
CFLAGS = -Wall -std=c++11 -O2

# Header files directories, example:
# INCLUDES = -I/home/newhall/include  -I../include
INCLUDES =  

# Library paths in addition to /usr/lib, for example
#   LFLAGS = -L/home/newhall/lib  -L../lib
LFLAGS =

# Libraries to link into executable, for example, 
# to link with libmylib.so and libm.so use:
# LIBS = -lmylib -lm
# -lpthread
LIBS = -lpthread

# All source files:
SRCS = $(wildcard *.cpp)

# All object files:
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
OBJS = $(SRCS:.cpp=.o)

# Executable file name:
MAIN = main

.PHONY: depend clean

all: $(MAIN)
	@echo $(MAIN) has been compiled!

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# This is a suffix replacement rule for building .o's from .cpp's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .cpp file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) $(OBJS) *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
